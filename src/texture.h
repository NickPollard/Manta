// texture.h
#include <gdkmm/pixbuf.h>

typedef Glib::RefPtr<Gdk::Pixbuf> pixbufHandle;

class Texture {
	public:

		Texture();
		Texture(const char* filename);
		~Texture();

		void Sample(float u, float v, color3f& color);

	protected:
		pixbufHandle	image;
		int width;
		int height;
		int channels;

};
