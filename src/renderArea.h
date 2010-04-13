#ifndef __RENDERAREA_H__
#define __RENDERAREA_H__

#include <gtkmm/drawingarea.h>

class View;

typedef Glib::RefPtr<Gdk::Pixbuf> pixbufHandle;

class RenderArea : public Gtk::DrawingArea
{
	public:
		// Constructors
		RenderArea();
		RenderArea(View* inRenderView, int width, int height);
		
		// Destructors
		virtual ~RenderArea();

		// Render the area based on current view data
		// This is called every frame to update the frame buffer
		bool render();

		// Calculate and render the pixel at the given coords
		void renderPixel(int x, int y);

	protected:
		pixbufHandle image;		// The pixbuf that holds the rendered image
		int		rowstride;		// Rowstride of the pixbuf, used in get/set pixel commands
		View*	renderView;		// View this renderArea represents

		vec2	center;			// Screen centre coordinates

		// Screen size properties
		int		renderWidth;
		int		renderHeight;

		float	invWidth;
		float	invHeight;

		float	imageWidth;
		float	imageHeight;
		float	aspectRatio;

		// Initialise values
		void init();


		// Determine whether the pixel at a given coord needs to be ray traced or can just be interpolated
		void shouldRayTrace(int x, int y);
		void shouldRayTraceB(int x, int y);

		// Set the colour of the pixel at the given coords on the pixbuf
		void setPixel(int x, int y, guint8 red, guint8 green, guint8 blue);
		// Get the colour of the pixel at the given coords on the pixbuf
		bool getPixel(color3f* color, int x, int y);

		// Override default signal handler
		virtual bool on_expose_event(GdkEventExpose* event);


};

struct renderArgs {
	RenderArea* area;
	int width;
	int height;
	int step;
	int base;
};

void* renderPass(void* args);

#endif // __RENDERAREA_H__
