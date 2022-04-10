#pragma once
#include <NumberEngine/Math/Vectors.hpp>
#include <NumberEngine/Math/Vertex.hpp>
#include <NumberEngine/Math/Matrix.hpp>
#include <NumberEngine/Math/CB.hpp>

namespace Num {

	APIEXPORT Real Sin(const Real& v);
	APIEXPORT Float Sinf(const Float& v);

	APIEXPORT Real Cos(const Real& v);
	APIEXPORT Float Cosf(const Float& v);

	APIEXPORT Real ASin(const Real& v);
	APIEXPORT Float ASinf(const Float& v);

	APIEXPORT Real ACos(const Real& v);
	APIEXPORT Float ACosf(const Float& v);

	APIEXPORT Real Abs(const Real& v);
	APIEXPORT Float Absf(const Float& v);

	APIEXPORT Bool FastComp(const Real& a, const Real& b);
	APIEXPORT Bool FastCompf(const Float& a, const Float& b);
	APIEXPORT Bool FastCompf(const Float& a, const Float& b, Int maxUlpsDiff);

	APIEXPORT Real Sqrt(const Real& n);
	APIEXPORT Float Sqrtf(const Float& n);

}
