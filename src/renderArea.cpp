#include "src/common.h"
#include "src/renderArea.h"
//--------------------------------------------------------
#include "src/scene.h"
#include "src/vector.h"
#include "src/view.h"

//--------------------------------------------------------
RenderArea::RenderArea()
{
    init();
}

//--------------------------------------------------------
RenderArea::RenderArea(View* inRenderView, int width, int height) :
	renderView(inRenderView),
	renderWidth(width),
	renderHeight(height)
{
	init();
}

//--------------------------------------------------------
RenderArea::~RenderArea()
{
}

//--------------------------------------------------------
void RenderArea::init()
{
	center.x = renderWidth * 0.5f;
	center.y = renderHeight * 0.5f;

	invWidth = 1.f / (float)renderWidth;
	invHeight = 1.f / (float)renderHeight;

	imageWidth = 10.f;
	aspectRatio = 0.75f;
	imageHeight = imageWidth * aspectRatio;
	// Create the pixbuffer we render to
	image = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, 0, 8, renderWidth, renderHeight);
	image->fill(0xFFFFFF00);
	rowstride = image->get_rowstride();
}

//--------------------------------------------------------
bool RenderArea::render()
{
	PROFILE_MASTER_BEGIN(PROFILE_RENDER);

	PROFILE_BEGIN(PROFILE_RENDER_PREPARE);
	renderView->prepareForRender();
	PROFILE_END(PROFILE_RENDER_PREPARE);

#if THREEPHASE_RENDER
	// 3-phase render
	// phase 1
	int i = 0;
	int j = 0;
	PROFILE_BEGIN(PROFILE_RENDER_PHASE_1)
	for (i = 0; i < renderWidth; i+=2) {
		for (j = 0; j < renderHeight; j+=2) {
			renderPixel(i, j); } }
	PROFILE_END(PROFILE_RENDER_PHASE_1)
	// phase 2
	PROFILE_BEGIN(PROFILE_RENDER_PHASE_2)
	for (i = 1; i < renderWidth; i+=2) {
		for (j = 1; j < renderHeight; j+=2) {
			shouldRayTraceB(i, j); } }
	PROFILE_END(PROFILE_RENDER_PHASE_2)
	// phase 3
	PROFILE_BEGIN(PROFILE_RENDER_PHASE_3)
	for (i = 0; i < renderWidth; i++) {
		j = ~(i & 0x1) & 0x1;
		for (; j < renderHeight; j+=2) {
			shouldRayTrace(i, j); } }
	PROFILE_END(PROFILE_RENDER_PHASE_3)

#elif TWOPHASE_RENDER
	// 2-phase render
	// phase 1
	int i = 0;
	int j = 0;
	PROFILE_BEGIN(PROFILE_RENDER_PHASE_1)

	for (i = 0; i < renderWidth; i++) {
		j = (i & 0x1);
		for (; j < renderHeight; j+=2) {
			renderPixel(i, j); } }

	PROFILE_END(PROFILE_RENDER_PHASE_1)
	// phase 2
	PROFILE_BEGIN(PROFILE_RENDER_PHASE_2)

	for (i = 0; i < renderWidth; i++) {
		j = ~(i & 0x1) & 0x1;
		for (; j < renderHeight; j+=2) {
			shouldRayTrace(i, j); } }

	PROFILE_END(PROFILE_RENDER_PHASE_2)

#else
		// 1-phase render
#define THREADED_RENDER 1
#if THREADED_RENDER
	static const int kThreadCount = 1;
	pthread_t thread[kThreadCount];
	renderArgs args[kThreadCount];

	repeat(kThreadCount) {
		args[i].width = renderWidth;
		args[i].height = renderHeight;
		args[i].step = kThreadCount;
		args[i].base = i;
		args[i].area = this; }
	
	repeat(kThreadCount) {
		int ret = pthread_create(&thread[i], NULL, renderPass, (void*)&args[i]); }

	repeat(kThreadCount) {
		pthread_join(thread[i], NULL); }

#else

	for (int i = 0; i < renderWidth; i+=2) {
		for (int j = 0; j < renderHeight; j+=2) {
			renderPixel(i, j); } }

#endif // THREADED_RENDER

#endif // 1-threaded render

	PROFILE_MASTER_END(PROFILE_RENDER);

	// Display the pixBuf on the drawingArea
	image->render_to_drawable(get_window(), 
			get_style()->get_black_gc(),
			0, 0, 0, 0,
			image->get_width(), image->get_height(),
			Gdk::RGB_DITHER_NONE, 0, 0);

	return true;
}

void* renderPass(void* args) {
	renderArgs& rArgs = *(renderArgs*)args;
	for (int x = rArgs.base; x < rArgs.width; x += rArgs.step) {
		for (int y = 0; y < rArgs.height; y++) {
			rArgs.area->renderPixel(x, y); } } 
	return NULL; }

//--------------------------------------------------------
void RenderArea::shouldRayTrace(int x, int y)
{
	// Calculate max color variation
	// First get adjacent pixel colors
	guint8* pixels = image->get_pixels();
	guint8*	curPix;
	color3i colorA, colorB, totalColor;
	int parts = 0;
	bool variant = false;
	if ((x > 0) && (x < renderWidth-1))
	{
		// Compare Horizontal
		curPix = pixels;
		curPix+= (rowstride * y);
		curPix+= x*3 - 3;
		colorA.red	 = (*curPix);
		colorA.green = (*(curPix+1));
		colorA.blue	 = (*(curPix+2));
		curPix+= 6;
		colorB.red	 = (*curPix);
		colorB.green = (*(curPix+1));
		colorB.blue	 = (*(curPix+2));
		if (colorIntVariance(&colorA, &colorB) > MAX_COLOR_VARIATION_SQUARE)
		{
			variant = true;
		}
		else
		{
			colorIntAdd(&totalColor, &colorA, &colorB);
			parts+=2;
		}
	}
	if ((!variant) && (y > 0) && (y < renderHeight-1))
	{
		// Compare Vertical
		curPix = pixels;
		curPix+= (rowstride * (y-1));
		curPix+= x*3;
		colorA.red	 = (*curPix);
		colorA.green = (*(curPix+1));
		colorA.blue	 = (*(curPix+2));
		curPix+= rowstride * 2;
		colorB.red	 = (*curPix);
		colorB.green = (*(curPix+1));
		colorB.blue	 = (*(curPix+2));
		if (colorIntVariance(&colorA, &colorB) > MAX_COLOR_VARIATION_SQUARE)
		{
			variant = true;
		}
		else
		{
			colorIntAdd(&totalColor, &totalColor, &colorA);
			colorIntAdd(&totalColor, &totalColor, &colorB);
			parts+=2;
		}
	}
	// If difference between 4 surrounding points too big
	if (variant)
	{
		renderPixel(x, y);
		return;
	}
	// Otherwise average surrounding colors
	else if (parts)
	{
		colorIntDivide(&totalColor, parts);
		curPix = pixels;
		curPix+= (rowstride * y);
		curPix+= x*3;
		(*curPix)	  = totalColor.red;
		(*(++curPix)) = totalColor.green;
		(*(++curPix)) = totalColor.blue;
	}
}

//--------------------------------------------------------
void RenderArea::shouldRayTraceB(int x, int y)
{
	// Calculate max color variation
	// First get adjacent pixel colors
	guint8* pixels = image->get_pixels();
	guint8*	curPix;
	color3i colorA, colorB, totalColor;
	int parts=0;
	bool variant = true;
	if ((x > 0) && (x < renderWidth-1))
	{
		if ((y > 0) && (y < renderHeight-1))
		{
			variant = false;
			// Compare Horizontal
			curPix = pixels;
			curPix+= (rowstride * (y-1));
			curPix+= (x*3) - 3;
			colorA.red	 = (*curPix);
			colorA.green = (*(curPix+1));
			colorA.blue	 = (*(curPix+2));
			curPix+= 6 + (rowstride * 2);
			colorB.red	 = (*curPix);
			colorB.green = (*(curPix+1));
			colorB.blue	 = (*(curPix+2));
			if (colorIntVariance(&colorA, &colorB) > MAX_COLOR_VARIATION_SQUARE)
			{
				variant = true;
			}
			else
			{
				colorIntAdd(&totalColor, &colorA, &colorB);
				parts+=2;
			}
			// Compare Vertical
			curPix = pixels;
			curPix+= (rowstride * (y-1));
			curPix+= (x*3) + 3;
			colorA.red	 = (*curPix);
			colorA.green = (*(curPix+1));
			colorA.blue	 = (*(curPix+2));
			curPix+= (rowstride * 2) - 6;
			colorB.red	 = (*curPix);
			colorB.green = (*(curPix+1));
			colorB.blue	 = (*(curPix+2));
			if (colorIntVariance(&colorA, &colorB) > MAX_COLOR_VARIATION_SQUARE)
			{
				variant = true;
			}
			else
			{
				colorIntAdd(&totalColor, &totalColor, &colorA);
				colorIntAdd(&totalColor, &totalColor, &colorB);
				parts+=2;
			}
		}
	}

	// If difference between 4 surrounding points too big
	if (variant)
	{
		renderPixel(x, y);		
		return;
	}
	// Otherwise average surrounding colors
	else if (parts)
	{
		colorIntDivide(&totalColor, parts);
		curPix = pixels;
		curPix+= (rowstride * y);
		curPix+= x*3;
		(*curPix)	  = totalColor.red;
		(*(++curPix)) = totalColor.green;
		(*(++curPix)) = totalColor.blue;
	}
}

//--------------------------------------------------------
void RenderArea::renderPixel(int x, int y)
{
	manta_assert(x >= 0, "renderPixel: x < 0");
	manta_assert(x < renderWidth, "renderPixel: x > width");
	manta_assert(y >= 0, "renderPixel: y < 0");
	manta_assert(y < renderHeight, "renderPixel, y > height");

	color3f pixCol;
	color3i intCol;
    // TODO - remove this cast
	float fx = (float) x;
	float fy = (float) y;

	// Convert pixel coordinates to the range -0.5<x<0.5
	fx = ((fx - center.x) * invWidth);
	// Negate Y coord since screen origin is topleft not bottomleft
	fy = ((center.y - fy) * invHeight);

	renderView->rayTrace(pixCol, fx, fy);
#if MULTISAMPLE_ENABLE
	float dx = 0.5f * invWidth ;
	float dy = 0.5f * invHeight ;

	color3f endCol(0.f, 0.f, 0.f);
	vec3Add(&endCol, &endCol, &pixCol);
	
	fx += dx;
	renderView->rayTrace(pixCol, fx, fy);
	vec3Add(&endCol, &endCol, &pixCol);

	fy += dx;	
	renderView->rayTrace(pixCol, fx, fy);
	vec3Add(&endCol, &endCol, &pixCol);
	
	fx -= dy;	
	renderView->rayTrace(pixCol, fx, fy);
	vec3Add(&endCol, &endCol, &pixCol);

	vec3Scale(&endCol, 0.25f);

	colorFloatToInt(&intCol, &endCol);
#else
	colorFloatToInt(&intCol, &pixCol);
#endif
	setPixel(x, y, intCol.red, intCol.green, intCol.blue);

	return;
}

//--------------------------------------------------------
void RenderArea::setPixel(int x, int y, guint8 red, guint8 green, guint8 blue)
{
	PROFILE_BEGIN(PROFILE_SETPIXEL);
	guint8* pixels = image->get_pixels();
	pixels+= (rowstride * y);
	pixels+= x*3;
	(*pixels)	  = red;
	(*(++pixels)) = green;
	(*(++pixels)) = blue;
	PROFILE_END(PROFILE_SETPIXEL);
}

//--------------------------------------------------------
bool RenderArea::getPixel(color3f* color, int x, int y)
{
	if ((x >= 0) && (y >= 0) && (x < renderWidth) && (y < renderHeight))
	{
		guint8* pixels = image->get_pixels();
		pixels+= (rowstride * y);
		pixels+= x*3;
		color3i intColor;
		intColor.red	= (*pixels);
		intColor.green	= (*(++pixels));
		intColor.blue	= (*(++pixels));
		colorIntToFloat(color, &intColor);
		return true;
	}
	return false;
}

//--------------------------------------------------------
bool RenderArea::on_expose_event(GdkEventExpose* event)
{
	image->render_to_drawable(get_window(), 
			get_style()->get_black_gc(),
			0, 0, 0, 0,
			image->get_width(), image->get_height(),
			Gdk::RGB_DITHER_NONE, 0, 0);
	return true;
}
