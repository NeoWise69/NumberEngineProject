#pragma once
#include <NumberEngine/Minimal.hpp>
#include <NumberEngine/Math/Vectors.hpp>

namespace Num {

	struct APIEXPORT mat4x4 {
		Float data[4][4];

		void identity();
		void translate(const vec3& pos);
		void translate(const vec2& pos);
		void rotateX(Float x);
		void rotateY(Float y);
		void rotateZ(Float z);
		void rotate(const vec3& vec);
		void rotate(const vec2& vec);
		void scale(const vec3& s);
		void scale(const vec2& s);
		void orthographic(Float w, Float h, Float znear, Float zfar);
		void perspective(Float fov, Float aspect, Float znear, Float zfar);
		vec3 getTranslation();
		vec3 getXDirection();
		vec3 getZDirection();
		Float determinant();
		void inverse();

		
		void operator*=(const mat4x4& mat);
		void operator+=(const mat4x4& mat);
		void operator-=(const mat4x4& mat);
		void operator/=(const mat4x4& mat);
	};


}
