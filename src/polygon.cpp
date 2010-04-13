#include "src/common.h"
#include "src/polygon.h"

volume3 polygon::getBounds() {
	volume3 bounds;
	ForEachAxis() {
		bounds.min[axis] = fmin3(GetA()[axis], GetB()[axis], GetC()[axis]);
		bounds.max[axis] = fmax3(GetA()[axis], GetB()[axis], GetC()[axis]);
	}
	return bounds;
}
