#ifndef COMMONDEFS_H
#define COMMONDEFS_H

#define EPSILON_FOR_COMPARE (1e-15)

#include "utils/Geometry.h"

namespace R2Geometry {
	using Vector = Geometry::Vector<2>;
	using Point = Geometry::Point<2>;
}

namespace R3Geometry {
	using Vector = Geometry::Vector<3>;
	using Point = Geometry::Point<3>;
}

#endif // COMMONDEFS_H
