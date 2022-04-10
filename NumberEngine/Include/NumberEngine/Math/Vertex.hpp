#pragma once
#include <NumberEngine/Minimal.hpp>
#include <NumberEngine/Math/Vectors.hpp>

namespace Num {

	struct PositionVertex {
		vec3 Position;
	};

	struct PositionColorVertex {
		vec3 Position;
		vec3 Color;
	};

	struct PositionPosition1ColorVertex {
		vec3 Position;
		vec3 Position1;
		vec3 Color;
	};

	struct PositionColorColor1Vertex {
		vec3 Position;
		vec3 Color;
		vec3 Color1;
	};

	struct PositionPosition1ColorColor1Vertex {
		vec3 Position;
		vec3 Position1;
		vec3 Color;
		vec3 Color1;
	};

}
