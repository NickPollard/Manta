// texture.cpp
#include "src/common.h"
#include "src/texture.h"

Texture::Texture() :
	image(NULL),
	width(0),
	height(0)
{
}

Texture::Texture(const char* filename) {
	image = Gdk::Pixbuf::create_from_file(filename);
	width = image->get_width();
	height = image->get_height();
	channels = image->get_n_channels();
	int bitCount = image->get_bits_per_sample();

	printf("Texture: Loaded image with %d channels and %d bits per sample.\n", channels, bitCount);
//	manta_assert(0, "");
}

Texture::~Texture() {
}

// U and V should be floats between 0.f and 1.f
void Texture::Sample(float u, float v, color3f& color) {
	/*
	color.red = u;
	color.green = v;
	color.blue = 0.f;
	*/

	manta_assert(((u >= 0.f) && (u <= 1.f) && (v >= 0.f) && (v <= 1.f)), "Texture sampling out of bounds");
	int x = (int)(u * width);
	int y = (int)(v * height);
	guint8* pixels = image->get_pixels();
	pixels += image->get_rowstride() * y; 
	pixels += channels * x;

//	printf("Sampling u: %2f v: %2f (As ints): u: %d v: %d (width: %d height: %d).\n", u, v, (int)(u * width), (int)(v * height), width, height);

	color = ColorFromG8(pixels);
}
