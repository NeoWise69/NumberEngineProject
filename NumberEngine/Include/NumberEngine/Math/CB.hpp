#pragma once
#include <NumberEngine/Minimal.hpp>
#include <NumberEngine/Math/Vectors.hpp>
#include <NumberEngine/Math/Matrix.hpp>

namespace Num {

	__declspec(align(16))
	struct CB {
		mat4x4 m_WorldMatrix;
		mat4x4 m_ViewMatrix;
		mat4x4 m_ProjectionMatrix;
		UInt m_Time;
	};

}
