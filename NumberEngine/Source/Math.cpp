#include <NumberEngine/Math/Math.hpp>

#include <math.h>

namespace Num {

	union Float_t
	{
		Float_t(Float num = 0.0f) : f(num) {}

		Bool Negative() const { return (i >> 31) != 0; }
		Int RawMantissa() const { return i & ((1 << 23) - 1); }
		Int RawExponent() const { return (i >> 23) & 0xFF; }

		Int i;
		Float f;
	};

	Real Sin(Real v) {
		return ::sin(v);
	}

	Float Sinf(Float v) {
		return ::sinf(v);
	}

	Real Cos(Real v) {
		return ::cos(v);
	}

	Float Cosf(Float v) {
		return ::cosf(v);
	}

	Real ASin(Real v) {
		return ::asin(v);
	}

	Float ASinf(Float v) {
		return ::asinf(v);
	}

	Real ACos(Real v) {
		return ::acos(v);
	}

	Float ACosf(Float v) {
		return ::acosf(v);
	}

	Real Abs(Real v) {
		return ::fabs(v);
	}

	Float Absf(Float v) {
		return ::fabsf(v);
	}

	Bool FastComp(const Real& a, const Real& b) {
		return (::fabs(a - b) <= DBL_EPSILON) ? true : false;
	}

	Bool FastCompf(const Float& a, const Float& b) {
		return (::fabsf(a - b) <= FLT_EPSILON) ? true : false;
	}

	Bool FastCompf(const Float& a, const Float& b, Int maxUlpsDiff) {
		Float_t uA(a);
		Float_t uB(b);

		if (uA.Negative() != uB.Negative()) {
			if (FastCompf(a, b))
				return true;
			return false;
		}

		Int ulpsDiff = ::abs(uA.i - uB.i);
		if (ulpsDiff <= maxUlpsDiff)
			return true;
		return false;
	}

	Real Sqrt(const Real& n) {
		return ::sqrt(n);
	}

	Float FastSqrt(const Float& n) {
		UInt i = *(UInt*)&n;
		// adjust bias
		i += 127 << 23;
		// approximation of square root
		i >>= 1;
		return *(Float*)&i;
	}

}
