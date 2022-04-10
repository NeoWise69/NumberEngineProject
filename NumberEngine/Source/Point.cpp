#include <NumberEngine/Minimal.hpp>

namespace Num {

	Bool operator==(const Point& p0, const Point& p1) {
		return (Bool)(
			p0.x == p1.x &&
			p0.y == p1.y
			);
	}

	Bool operator!=(const Point& p0, const Point& p1) {
		return (Bool)(!(p0 == p1));
	}

}
