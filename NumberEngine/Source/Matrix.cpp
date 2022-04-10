#include <NumberEngine/Math/Matrix.hpp>
#include <memory.h>
#include <math.h>

namespace Num {

	void mat4x4::identity() {
		::memset(data, 0, sizeof(Float) * 16);
		data[0][0] = 1;
		data[1][1] = 1;
		data[2][2] = 1;
		data[3][3] = 1;
	}

	void mat4x4::translate(const vec3& pos) {
		data[3][0] = pos.x;
		data[3][1] = pos.y;
		data[3][2] = pos.z;
	}

	void mat4x4::translate(const vec2& pos) {
		data[3][0] = pos.x;
		data[3][1] = pos.y;
	}

	void mat4x4::rotateX(Float x) {
		data[1][1] =  cosf(x);
		data[1][2] =  sinf(x);
		data[2][1] = -sinf(x);
		data[2][2] =  cosf(x);
	}

	void mat4x4::rotateY(Float y) {
		data[0][0] =  cosf(y);
		data[0][2] = -sinf(y);
		data[2][0] =  sinf(y);
		data[2][2] =  cosf(y);
	}

	void mat4x4::rotateZ(Float z) {
		data[0][0] =  cosf(z);
		data[0][1] =  sinf(z);
		data[1][0] = -sinf(z);
		data[1][1] =  cosf(z);
	}

	void mat4x4::rotate(const vec3& vec) {
		rotateZ(vec.z);
		rotateY(vec.y);
		rotateX(vec.x);
	}

	void mat4x4::rotate(const vec2& vec) {
		rotateX(vec.x);
		rotateY(vec.y);
	}

	void mat4x4::scale(const vec3& s) {
		data[0][0] = s.x;
		data[1][1] = s.y;
		data[2][2] = s.z;
	}

	void mat4x4::scale(const vec2& s) {
		data[0][0] = s.x;
		data[1][1] = s.y;
	}

	void mat4x4::orthographic(Float w, Float h, Float znear, Float zfar) {
		data[0][0] = 2.0f / w;
		data[1][1] = 2.0f / h;
		data[2][2] = 1.0f / (zfar - znear);
		data[3][2] = - (znear / (zfar - znear));
	}

	void mat4x4::perspective(Float fov, Float aspect, Float znear, Float zfar) {
		Float yscale = 1.0f / tanf(fov / 2.0f);
		Float xscale = yscale / aspect;

		data[0][0] = xscale;
		data[1][1] = yscale;
		data[2][2] = zfar / (zfar - znear);
		data[2][3] = 1.0f;
		data[3][2] = (-znear * zfar) / (zfar - znear);
	}

	vec3 mat4x4::getTranslation() {
		return {
			(data[3][0]), (data[3][1]), (data[3][2])
		};
	}

	vec3 mat4x4::getXDirection() {
		return vec3((data[0][0]), (data[0][1]), (data[0][2]));
	}

	vec3 mat4x4::getZDirection() {
		return vec3((data[2][0]),  (data[2][1]), (data[2][2]));
	}

	Float mat4x4::determinant()
	{
		vec4 minor, v1, v2, v3;
		Float det;

		v1 = { this->data[0][0], this->data[1][0], this->data[2][0], this->data[3][0] };
		v2 = { this->data[0][1], this->data[1][1], this->data[2][1], this->data[3][1] };
		v3 = { this->data[0][2], this->data[1][2], this->data[2][2], this->data[3][2] };


		minor.cross(v1, v2, v3);
		det = -(this->data[0][3] * minor.x + this->data[1][3] * minor.y + this->data[2][3] * minor.z +
			this->data[3][3] * minor.w);
		return det;
	}

	void mat4x4::inverse()
	{
		Int a, i, j;
		mat4x4 out;
		vec4 v, vec[3];
		Float det = 0.0f;

		det = this->determinant();
		if (!det) return;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i) a = a - 1;
					vec[a].x = (this->data[j][0]);
					vec[a].y = (this->data[j][1]);
					vec[a].z = (this->data[j][2]);
					vec[a].w = (this->data[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.data[0][i] = powf(-1.0f, (Float)(i)) * v.x / det;
			out.data[1][i] = powf(-1.0f, (Float)(i)) * v.y / det;
			out.data[2][i] = powf(-1.0f, (Float)(i)) * v.z / det;
			out.data[3][i] = powf(-1.0f, (Float)(i)) * v.w / det;
		}

		*this = (out);
	}

	void mat4x4::operator*=(const mat4x4& mat) {
		mat4x4 out;
		for (Int i = 0; i < 4; i++)
			for (Int j = 0; j < 4; j++)
				out.data[i][j] = 
					data[i][0] * mat.data[0][j] + data[i][1] * mat.data[1][j] +
					data[i][2] * mat.data[2][j] + data[i][3] * mat.data[3][j];
		::memcpy(data, out.data, 64);
	}

	void mat4x4::operator+=(const mat4x4& mat) {
		mat4x4 out;
		for (Int i = 0; i < 4; i++)
			for (Int j = 0; j < 4; j++)
				out.data[i][j] =
				data[i][0] + mat.data[0][j] + data[i][1] + mat.data[1][j] +
				data[i][2] + mat.data[2][j] + data[i][3] + mat.data[3][j];
		::memcpy(data, out.data, 64);
	}

	void mat4x4::operator-=(const mat4x4& mat) {
		mat4x4 out;
		for (Int i = 0; i < 4; i++)
			for (Int j = 0; j < 4; j++)
				out.data[i][j] =
				data[i][0] - mat.data[0][j] + data[i][1] - mat.data[1][j] +
				data[i][2] - mat.data[2][j] + data[i][3] - mat.data[3][j];
		::memcpy(data, out.data, 64);
	}

	void mat4x4::operator/=(const mat4x4& mat) {
		mat4x4 out;
		for (Int i = 0; i < 4; i++)
			for (Int j = 0; j < 4; j++)
				out.data[i][j] =
				data[i][0] / mat.data[0][j] + data[i][1] / mat.data[1][j] +
				data[i][2] / mat.data[2][j] + data[i][3] / mat.data[3][j];
		::memcpy(data, out.data, 64);
	}

}
