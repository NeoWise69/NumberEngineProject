#pragma once
#include <NumberEngine/Minimal.hpp>
//#include <intrin.h>

namespace Num {

	struct APIEXPORT vec2 {
		Float x, y;

		vec2() : x(0), y(0) {}
		vec2(const vec2&) = default;
		vec2(Float x_, Float y_) : x(x_), y(y_) {}
		
		void lerp(const vec2& p1, Float dt);
		
		vec2 operator*(const vec2& v1);
		vec2 operator*(const Float& f);
		vec2 operator+(const vec2& v1);
		vec2 operator+(const Float& f);
		vec2 operator-(const vec2& v1);
		vec2 operator-(const Float& f);
		vec2 operator/(const vec2& v1);
		vec2 operator/(const Float& f);
		Bool operator==(const vec2& v1);
		Bool operator!=(const vec2& v1);
	};

	struct APIEXPORT vec3 {
		Float x, y, z;

		vec3() : x(0), y(0), z(0) {}
		vec3(const vec3&) = default;
		vec3(Float x_, Float y_, Float z_) : x(x_), y(y_), z(z_) {}
		
		void lerp(const vec3& p1, Float dt);
		
		vec3 operator*(const vec3& v1);
		vec3 operator*(const Float& f);
		vec3 operator+(const vec3& v1);
		vec3 operator+(const Float& f);
		vec3 operator-(const vec3& v1);
		vec3 operator-(const Float& f);
		vec3 operator/(const vec3& v1);
		vec3 operator/(const Float& f);
		Bool operator==(const vec3& v1);
		Bool operator!=(const vec3& v1);
	};

	struct APIEXPORT vec4 {
		Float x, y, z, w;

		vec4() : x(0), y(0), z(0), w(0) {}
		vec4(const vec4&) = default;
		vec4(Float x_, Float y_, Float z_, Float w_) : x(x_), y(y_), z(z_), w(w_) {}
		
		void lerp(const vec4& p1, Float dt);
		void cross(const vec4& p0, const vec4& p1, const vec4& p2);
		
		vec4 operator*(const vec4& v1);
		vec4 operator*(const Float& f);
		vec4 operator+(const vec4& v1);
		vec4 operator+(const Float& f);
		vec4 operator-(const vec4& v1);
		vec4 operator-(const Float& f);
		vec4 operator/(const vec4& v1);
		vec4 operator/(const Float& f);
		Bool operator==(const vec4& v1);
		Bool operator!=(const vec4& v1);
	};




}
