#ifndef __KDTREE_H__
#define __KDTREE_H__

//---------------------------------------------------------------------------
// Misc defines for this test setup

#define MAX_KDNODES 512
#define MAX_TREEPOLYS 2048

// Forward Declarations
class kdTree;
class volume3;

enum masks {
	kPointer = 0xFFFFFFFFFFFFFFFE,	// 64 bit
	kIsLeaf = 0x0000000000000001
};

//---------------------------------------------------------------------------
enum eventType {
	kPlanar,
	kAdd,
	kRemove,
};

struct splitEvent {
	float		v;			//	 axis Value;
	polygon*	triangle;
	eventType	type;

	splitEvent(eventType typeIn, float vIn) :
		v(vIn),
		type(typeIn)
	{}

	bool operator < (const splitEvent other) const {
		return (v < other.v);
	}
};

typedef std::vector<splitEvent> eventList;

//---------------------------------------------------------------------------
// KdNode class - stores one node of the kdTree
// has an 8-byte size for ease and multitude of storage
class kdNode
{

	public:
		kdNode()	:
			split(0.f),
			data(NULL)
	{}

		//---------------------------------------------------------------------------
		// Access split plane value
		const float	Split()	const		{	return split;	}
		void		SetSplit(float s)	{	split = s;		}
		//---------------------------------------------------------------------------

		//---------------------------------------------------------------------------
		// Access the data from this node
		kdNode*				GetFirstChild()			{	return (kdNode*)(data & kPointer);			}
		kdNode*				GetSecondChild()		{	return &((kdNode*)(data & kPointer))[1];	} // data always adjacent in nodelist
		const kdNode*		GetFirstChild()	 const	{	return (kdNode*)(data & kPointer);			}
		const kdNode*		GetSecondChild() const	{	return &((kdNode*)(data & kPointer))[1];	} // data always adjacent in nodelist
		slice<polygon*>&	GetTriList()			{	return *(slice<polygon*>*)(data & kPointer);	}
		const slice<polygon*>&	GetTriList() const	{	return *(slice<polygon*>*)(data & kPointer);	}
		void				SetPointer(void* pointer)	{	data = (data & ~kPointer) + ((u_long)pointer & kPointer);	}
		//---------------------------------------------------------------------------

		//---------------------------------------------------------------------------
		// Leaf node functions	
		bool	IsLeaf() const	{	return !!(data & kIsLeaf);	}
		void	SetAsLeaf()		{	data |= kIsLeaf;			}
		void	SetNotLeaf()	{	data &= ~kIsLeaf;			}
		//---------------------------------------------------------------------------

		//---------------------------------------------------------------------------
		// SAH/Build functions
		void	buildEventList(eventList& events, int axis) const;
		void	addEvent(eventList& events, eventType e, float value) const;
		void	processEvent(splitEvent event, int& pL, int& pP, int& pR) const;
		float	ScanForBestSplit(int axis, volume3 bounds, float& cheapestCost, bool& pre) const;
		float	CalculateSplitCost(volume3 bounds, int axis, float split, int triCountA, int triCountB) const;
		void	subDivideNode(kdTree* tree, int axis, volume3 bounds, int depth);
		//---------------------------------------------------------------------------

		void	Reset()	{	data = NULL; split = 0.f;	}

		// Poly list functions
		void	AddPoly(polygon* poly)	{	GetTriList().Append(poly);	}

	protected:
		float	split;		// The coordinate value of the split plane, coordinate axis is determined by tree depth
		u_long	data;		// Leaf-nodes - points to poly list, non-leaf nodes; points to first child
	// TODO - sort out 64bit issues
};

//---------------------------------------------------------------------------
class kdTree
{
	public:
		kdTree()	:
			size(0),
			polyCount(0)
		{}

		// Get a pointer to the root node
		kdNode*		Root()			{	return &kdNodeList[0];	}
		const kdNode* Root() const	{	return &kdNodeList[0];	}

		// Get a pointer to the next available slot
		kdNode*		Next()	{
			kdNode* result = &kdNodeList[size];
			result->SetPointer(&polyLists[size]);
			++size;
			return result;
		}

		// Get the tree size in total nodes
		int			Size() const	{	return size;	}

		// Get a pointer to the next available polygon slot in the poly list
		polygon**	GetNextFreePolygon()	{	return &polygons[polyCount];	}
		void		incPolyCount(int i)	{	polyCount += i; manta_assert(polyCount < MAX_TREEPOLYS, "Error: added too many polys to kdTree");	}

		void 	TraverseTree(rayData& ray, intersectData& intersect) const;
		void	Rebuild(polygon* scenePolys, int scenePolyCount);
		void	Reset();

		void	PrintStats() const;

	protected:

		// Nodes
		kdNode		kdNodeList[MAX_KDNODES];
		slice<polygon*>	polyLists[MAX_KDNODES];
		int			size;

		// Polys
		polygon*	polygons[MAX_TREEPOLYS];
		int			polyCount;
};


void 	kdRayTrace(const slice<polygon>& polyList, rayData& ray, intersectData& intersect);

void	Traverse(const kdNode* node, int axis, rayData& ray, float max, intersectData& intersect);

#endif // __KDTREE_H__

