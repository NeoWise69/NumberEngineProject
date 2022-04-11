#pragma once

#ifdef _DLL
#	define APIEXPORT __declspec(dllexport)
#	define NUM_DLL
#else
#	define APIEXPORT __declspec(dllimport)
#	define NUM_APP
#endif

#include <cfloat>
#include <string>
#include <memory>

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

	// Helpers for memory management
	template<class T>
	using Ref = std::shared_ptr<T>;

	template<class T, class...Args>
	constexpr Ref<T> CreateRef(Args... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<class T>
	using Unique = std::unique_ptr<T>;

	template<class T, class...Args>
	constexpr Unique<T> CreateUniqie(Args... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

}

#define ARRSIZE(_array_) (sizeof(_array_) / sizeof(_array_[0]))
#define NULL 0

// Pimpl mechanics
// See https://en.cppreference.com/w/cpp/language/pimpl for more information

#define NCLASS(name) class name ## Impl;
#define NCLASSEND(name) struct { Num::Ref<name ## Impl> data; } pimpl;
