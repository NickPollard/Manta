#include "src/common.h"
#include "src/kdtree.h"
//--------------------------------------------------------
#include "src/geometry.h"

#define KD_BUILD_PRINT_ENABLE 0
#define DRAW_TREE_DEPTH 0

#if KD_BUILD_PRINT_ENABLE
#define KD_BUILD_PRINT(fmt, args...)		printf(fmt, ## args)
#else
#define KD_BUILD_PRINT(...)		{}
#endif

// Tree bounds
static const float kMaxTreeBound = 20.f;

static const int kPolyCountThreshold = 8;

// SAH properties
static const float kTraversalCost = 1.f;	// TODO - set something good
static const float kTriangleCost = 2.f;
static const float kEmptyScalingFactor = 1.f;
static const float kSplitTestDelta = 0.1f;

static const int MAX_TEMP_POLYS = 256;

#if PROFILE_ENABLE == 1
static int totalPolysTested = 0;
static int nodesTraversed = 0;
static int raysTraced = 0;
static int leastPolysTested = 65536;
static int mostPolysTested = 0;
static int currentPolysTested = 0;
static int leafNodeCount = 0;
static int emptyLeafNodeCount = 0;
static int leafNodesTraversed = 0;
#endif // PROFILE_ENABLE == 1

//---------------------------------------------------------------------------
int		incrementAxis(int axis) {
	return (axis + 1) % 3;
}

//---------------------------------------------------------------------------
void kdRayTrace(const slice<polygon*>& polygons, rayData& ray, intersectData& intersectResult)
{
	float		closest = FLOAT_MAX;

#if PROFILE_ENABLE == 1
		++leafNodesTraversed;
#endif

	//	printf("Traversing leaf node with %d polys.\n", polygons.Length());

	PROFILE_END(PROFILE_TRAVERSE)
	PROFILE_BEGIN(PROFILE_TESTPOLY)

	repeat(polygons.Length()) {
		polygon* poly = polygons[i];
		point3	intersectPoint;
		float	depth = closest;
		float u = 0.f, v = 0.f;

		// Find point of intersection
		PROFILE_BEGIN(PROFILE_INTERSECT);
		bool hit = intersect(*poly, ray, intersectPoint, depth, u, v);
		PROFILE_END(PROFILE_INTERSECT);

		// If point of intersection closer than closest
		if (hit && (depth > 0.f) && (depth < closest)) {   
			closest = depth;
			intersectResult.poly = poly;
			intersectResult.location = intersectPoint;
			intersectResult.u = u;
			intersectResult.v = v;
		}

#if PROFILE_ENABLE == 1
		++currentPolysTested;
		++totalPolysTested;
#endif
	}

	PROFILE_END(PROFILE_TESTPOLY)
	PROFILE_BEGIN(PROFILE_TRAVERSE)
}

//---------------------------------------------------------------------------
// Begin traversing the tree
void kdTree::TraverseTree(rayData& ray, intersectData& intersect) const
{
#if PROFILE_ENABLE == 1
	currentPolysTested = 0;
#endif

	// TODO - optimise this away somehow, it's slow
	float ray_max = FLOAT_MAX;
	repeat (3)	{
		float a = (-kMaxTreeBound - ray.origin[i]) / ray.direction[i];
		float b = (kMaxTreeBound - ray.origin[i]) / ray.direction[i];
		ray_max = fmin(ray_max, fmax(a, b));
	}

	PROFILE_BEGIN(PROFILE_TRAVERSE)
	Traverse(Root(), kX, ray, ray_max, intersect); 
	PROFILE_END(PROFILE_TRAVERSE)

#if PROFILE_ENABLE == 1
	raysTraced++;

	mostPolysTested = max(mostPolysTested, currentPolysTested);
	leastPolysTested = min(leastPolysTested, currentPolysTested);
#endif // PROFILE_ENABLE == 1
}

//---------------------------------------------------------------------------
// Traverse - traverses the kdTree looking for the nearest intersection of a ray
// if an Intersection is found, stores the data in intersect for returning
void Traverse(const kdNode* node, int axis, rayData& ray, float ray_max, intersectData& intersect)		{
#if PROFILE_ENABLE == 1
	++nodesTraversed;
#endif

	if (!node->IsLeaf()) {
		const kdNode* first = NULL;
		const kdNode* second = NULL;
		float split;

		if (ray.direction[axis] != 0.f) {
			bool forward	= (ray.direction[axis] > 0.f);
			first	= forward ? node->GetFirstChild() : node->GetSecondChild();
			second	= forward ? node->GetSecondChild() : node->GetFirstChild();
			split = (node->Split() - ray.origin[axis]) / ray.direction[axis]; }
		else {
			first = (ray.origin[axis] > node->Split()) ? node->GetFirstChild() : node->GetSecondChild();
			split = ray_max + 1.f; } // force split > ray_max to ensure only one child is traversed

		axis = incrementAxis(axis);

		if (split <= 0.f)
			Traverse(second, axis, ray, ray_max, intersect);
		else if (split > ray_max)
			Traverse(first, axis, ray, ray_max, intersect);
		else {
			Traverse(first, axis, ray, split, intersect);
			if (intersect.poly) return;				// If we hit something, no need to go further
			rayData newRay;
			newRay.direction = ray.direction;
			newRay.origin = ray.origin + ray.direction * split;	// Advance the ray forward
			if (second)
				Traverse(second, axis, newRay, ray_max - split, intersect); } }
	else {
		kdRayTrace(node->GetTriList(), ray, intersect); }
}

//---------------------------------------------------------------------------
// Build a KDTree with polylists
void kdNode::subDivideNode(kdTree* tree, int axis, volume3 bounds, int depth)
{
	slice<polygon*>& list = GetTriList();
	KD_BUILD_PRINT("----Subdividing kdNode (on axis %d).\n", axis);
	KD_BUILD_PRINT("--node has poly list of size %d.\n", list.Length());

#if DRAW_TREE_DEPTH
	repeat(depth) {
		printf("--");
	}
#endif

	// Use Surface-Area-Heuristic to find cheapest split plane
	PROFILE_BEGIN(PROFILE_SAH)
	float cheapestCost = FLOAT_MAX;
	bool pre = false;
	float split = ScanForBestSplit(axis, bounds, cheapestCost, pre);
	PROFILE_END(PROFILE_SAH)

	// We split nodes if the split meets three conditions:
	// It is cheaper (by SAH) than not splitting
	// The current node has more than a given number of polys
	// the tree has room left for more nodes
	float currentCost = kTriangleCost * list.Length();
	if ( (currentCost > cheapestCost) && 
		 (list.Length() > kPolyCountThreshold) &&
		 (tree->Size() < (MAX_KDNODES - 2)) ) {
#if DRAW_TREE_DEPTH
		printf("\n");
#endif
		KD_BUILD_PRINT("--Splitting node at coord %.6f\n", split);
		SetSplit(split);

		// Build new nodes
		kdNode* first = tree->Next();
		kdNode* second = tree->Next();
		SetPointer(first);		

		// Allocate Polygons
		polygon*	firstList[MAX_TEMP_POLYS];
		polygon*	secondList[MAX_TEMP_POLYS];
		first->GetTriList().Relocate(firstList);
		second->GetTriList().Relocate(secondList);

		// Recurse
		volume3 firstBounds = bounds;
		volume3 secondBounds = bounds;

		firstBounds.max[axis] = split;
		secondBounds.min[axis] = split;

		if (pre) {
			repeat (list.Length()) {
				polygon* poly = list[i];
				if ((poly->getMin(axis) < split) && (overlap(*poly, firstBounds))) {
					first->AddPoly(poly);
				}
				if ((poly->getMax(axis) >= split) && (overlap(*poly, secondBounds))) {
					second->AddPoly(poly);
				}
			}
		} else {
			repeat (list.Length()) {
				polygon* poly = list[i];
				if ((poly->getMin(axis) <= split) && (overlap(*poly, firstBounds))) {
					first->AddPoly(poly);
				}
				if ((poly->getMax(axis) > split) && (overlap(*poly, secondBounds))) {
					second->AddPoly(poly);
				}
			}
		}
		axis = incrementAxis(axis);

//		printf("Splitting node with %d tris into nodes with %d and %d tris.\n", list.Length(), first->GetTriList().Length(), second->GetTriList().Length());

		first->subDivideNode(tree, axis, firstBounds, depth + 1);
		second->subDivideNode(tree, axis, secondBounds, depth + 1);
	}
	else
	{
#if DRAW_TREE_DEPTH
		printf("%d\n", list.Length());
#endif

		KD_BUILD_PRINT("--Setting as Leaf kdNode (%d polys)\n", list.Length());
		SetAsLeaf();

#if PROFILE_ENABLE == 1
		++leafNodeCount;
		if (list.Length() == 0)
			++emptyLeafNodeCount;
#endif

		// Copy polys to proper list in the tree
		list.Relocate(tree->GetNextFreePolygon());
		tree->incPolyCount(list.Length());	// We've added this many polys to the tree
	}
}

//---------------------------------------------------------------------------
void kdTree::Rebuild(polygon* scenePolys, int scenePolyCount)
{
	KD_BUILD_PRINT("Starting kdTree rebuild. Scene contains %d polys.\n", scenePolyCount);

	// Clear the tree
	Reset();

	// Set up the root node
	kdNode* root = Next();
	manta_assert(root == Root(), "Error constructing kdTree, root node not in situ");

	// Set up first poly list, should contain all scene polys
	slice<polygon> sceneList(scenePolys, scenePolyCount);
	slice<polygon*> newList(GetNextFreePolygon());
		
	repeat (sceneList.Length()) {
		newList.Append(&sceneList[i]);	}

	incPolyCount(newList.Length());	// We've added this many polys to the tree
	root->GetTriList() = newList;

	// initialise subdivision parameters
	volume3 sceneBounds;
	sceneBounds.min = point3(-kMaxTreeBound, -kMaxTreeBound, -kMaxTreeBound);
	sceneBounds.max = point3(kMaxTreeBound, kMaxTreeBound, kMaxTreeBound);

	// Recursively subdivide
	root->subDivideNode(this, kX, sceneBounds, 0);

#if PROFILE_ENABLE == 1
//	printf("tree size: %d nodes (%d leaf nodes, %d of which are empty), %d polygons\n", size, leafNodeCount, emptyLeafNodeCount, polyCount);
#endif
}

//---------------------------------------------------------------------------
void kdTree::Reset()
{
	repeat (MAX_KDNODES)	{
		kdNodeList[i].Reset();
		polyLists[i].Reset();
	}
	size = 0;
	polyCount = 0;

#if PROFILE_ENABLE == 1
	// Reset Debug tracking
	leafNodeCount = 0;
	emptyLeafNodeCount = 0;
#endif
}

//---------------------------------------------------------------------------
void kdTree::PrintStats() const
{
#if PROFILE_ENABLE == 1
	printf("---------- Tree traversal stats: -----------\n");
	printf("-- Total Rays Traced: %d\n", raysTraced);
	printf("-- Total Polygons tested: %d\n", totalPolysTested);
	printf("-- Average Polygons tested per ray %.2f\n", (float)totalPolysTested/(float)raysTraced);
	printf("-- Total Nodes Traversed: %d\n", nodesTraversed);
	printf("-- Average Nodes Traversed per ray %.2f\n", (float)nodesTraversed/(float)raysTraced);
	printf("-- Average Leaf Nodes Traversed per ray %.2f\n", (float)leafNodesTraversed/(float)raysTraced);
	printf("-- Most Polygons tested per ray %d\n", mostPolysTested);
	printf("-- Least Polygons tested per ray %d\n", leastPolysTested);
#endif // PROFILE_ENABLE == 1
}


//---------------------------------------------------------------------------
void kdNode::addEvent(eventList& events, eventType e, float value) const {
	events.push_back(splitEvent(e, value));
}

//---------------------------------------------------------------------------
void kdNode::processEvent(splitEvent event, int& pL, int& pP, int& pR) const {
	switch (event.type) {
		case kAdd:
			++pL;
			++pP;
			break;
		case kRemove:
			--pP;
			--pR;
			break;
		case kPlanar:
			break;
		default:
			break;
	}
}

//---------------------------------------------------------------------------
// Find min and max for each triangle for this axis
// Generate an +event for each min, and a -event for each max
// Sort events into order
void kdNode::buildEventList(eventList& events, int axis) const {
	const slice<polygon*>& triList = GetTriList();
	repeat (triList.Length()) {
		volume3 triBounds = triList[i]->getBounds();
		addEvent(events, kAdd, triBounds.min[axis]);
		addEvent(events, kRemove, triBounds.max[axis]);
	}
	std::sort(events.begin(), events.end());
//	printf("Built event list, size: %d\n", (int)events.size());
}

//---------------------------------------------------------------------------
float	kdNode::ScanForBestSplit(int axis, volume3 bounds, float& cheapestCost, bool& pre) const {
	eventList events;
	int i = 0;	// Event index
	int pL = 0, pP = 0;
	int pR = GetTriList().Length();
	float best = 0;

	PROFILE_BEGIN(PROFILE_BUILD_EVENTS)
	buildEventList(events, axis);
	PROFILE_END(PROFILE_BUILD_EVENTS)

	int length = events.size();
	
	if (length == 0)
		return 0.f;

	float p = bounds.min[axis];
	
	// Sweep forward through the event list
	while (i < length)	{
		// Evaluate the cost pre-events
		float preCost = CalculateSplitCost(bounds, axis, p, pL, pR);
		// Process all events with the same coordinate
		while ((i < length) && (events[i].v <= p)) {
			processEvent(events[i], pL, pP, pR);
			++i;
		}

		// Evaluate the cost post-events
		float postCost = CalculateSplitCost(bounds, axis, p, pL, pR);

		if (preCost < cheapestCost) {
			cheapestCost = preCost;
			best = p;
			pre = true;
		}
		if (postCost < cheapestCost) {
			cheapestCost = postCost;
			best = p;
			pre = false;
		}

		// Find next plane value;
		++i;
		p = events[i].v;
		if ((i >= length) || (p > bounds.max[axis]))
			break;
	}

	return best;
}

//---------------------------------------------------------------------------
float kdNode::CalculateSplitCost(volume3 bounds, int axis, float split, int triCountA, int triCountB) const {
	// Calculate surface area of proposed Children
	volume3 v1 = bounds;
	volume3 v2 = bounds;
	
	v1.max[axis] = split;
	v2.min[axis] = split;

	// Get current surface area
	float surfaceArea = bounds.SurfaceArea();

	// Calculate probabilities of recursion
	float p1 = v1.SurfaceArea() / surfaceArea;
	float p2 = v2.SurfaceArea() / surfaceArea;

	// Calculate total expected cost
	float cost = kTraversalCost + kTriangleCost * ((p1 * triCountA) + (p2 * triCountB));
	if ((triCountA == 0.f) || (triCountB == 0.f))
		cost *= kEmptyScalingFactor;
	return cost;
}
