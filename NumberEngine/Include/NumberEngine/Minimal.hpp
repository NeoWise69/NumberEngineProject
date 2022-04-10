#pragma once

#ifdef _DLL
#	define APIEXPORT __declspec(dllexport)
#else
#	define APIEXPORT __declspec(dllimport)
#endif

#include <cfloat>
#include <string>

namespace Num {

	using Char = char;
	using UChar = unsigned char;

	using Short = short;
	using UShort = unsigned short;

	using Int = int;
	using UInt = unsigned int;

	using Long = long;
	using LLong = long long;
	using ULong = unsigned long;
	using ULLong = unsigned long long;

	using Float = float;
	using Real = double;
	using BigReal = long double;

	using Bool = bool;

	using CString = const Char*;
	using String = std::string;

	struct Point {
		Long x, y;

		Point() : x(0), y(0) {}
		Point(Long x_, Long y_) : x(x_), y(y_) {}
		//Point(Float x_, Float y_) : x((Real)(x_)), y((Real)(y_)) {}
	};

	APIEXPORT Bool operator==(const Point& p0, const Point& p1);
	APIEXPORT Bool operator!=(const Point& p0, const Point& p1);

}

#define ARRSIZE(_array_) (sizeof(_array_) / sizeof(_array_[0]))
