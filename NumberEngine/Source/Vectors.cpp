#include <NumberEngine/Math/Vectors.hpp>

namespace Num {

    void vec2::lerp(const vec2& p1, Float dt) {
        x = x * (1.0f - dt) + x * (dt);
        y = y * (1.0f - dt) + y * (dt);
    }

    void vec3::lerp(const vec3& p1, Float dt) {
        x = x * (1.0f - dt) + x * (dt);
        y = y * (1.0f - dt) + y * (dt);
        z = z * (1.0f - dt) + z * (dt);
    }

    void vec4::lerp(const vec4& p1, Float dt) {
        x = x * (1.0f - dt) + x * (dt);
        y = y * (1.0f - dt) + y * (dt);
        z = z * (1.0f - dt) + z * (dt);
        w = w * (1.0f - dt) + w * (dt);
    }

    void vec4::cross(const vec4& p0, const vec4& p1, const vec4& p2) {
        x = p0.y * (p1.z * p2.w - p2.z * p1.w) - p0.z * (p1.y * p2.w - p2.y * p1.w) + p0.w * (p1.y * p2.z - p1.z * p2.y);
        y = -(p0.x * (p1.z * p2.w - p2.z * p1.w) - p0.z * (p1.x * p2.w - p2.x * p1.w) + p0.w * (p1.x * p2.z - p2.x * p1.z));
        z = p0.x * (p1.y * p2.w - p2.y * p1.w) - p0.y * (p1.x * p2.w - p2.x * p1.w) + p0.w * (p1.x * p2.y - p2.x * p1.y);
        w = -(p0.x * (p1.y * p2.z - p2.y * p1.z) - p0.y * (p1.x * p2.z - p2.x * p1.z) + p0.z * (p1.x * p2.y - p2.x * p1.y));
    }

    vec2 vec2::operator*(const vec2& v1) {
        x *= v1.x;
        y *= v1.y;
        return *this;
    }

    vec3 vec3::operator*(const vec3& v1) {
        x *= v1.x;
        y *= v1.y;
        z *= v1.z;
        return *this;
    }

    vec4 vec4::operator*(const vec4& v1) {
        x *= v1.x;
        y *= v1.y;
        z *= v1.z;
        w *= v1.w;
        return *this;
    }

    vec2 vec2::operator*(const Float& f) {
        x *= f;
        y *= f;
        return *this;
    }

    vec3 vec3::operator*(const Float& f) {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    vec4 vec4::operator*(const Float& f) {
        x *= f;
        y *= f;
        z *= f;
        w *= f;
        return *this;
    }

    vec2 vec2::operator+(const vec2& v1) {
        x += v1.x;
        y += v1.y;
        return *this;
    }

    vec3 vec3::operator+(const vec3& v1) {
        x += v1.x;
        y += v1.y;
        z += v1.z;
        return *this;
    }

    vec4 vec4::operator+(const vec4& v1) {
        x += v1.x;
        y += v1.y;
        z += v1.z;
        w += v1.w;
        return *this;
    }

    vec2 vec2::operator+(const Float& f) {
        x += f;
        y += f;
        return *this;
    }

    vec3 vec3::operator+(const Float& f) {
        x += f;
        y += f;
        z += f;
        return *this;
    }

    vec4 vec4::operator+(const Float& f) {
        x += f;
        y += f;
        z += f;
        w += f;
        return *this;
    }

    vec2 vec2::operator-(const vec2& v1) {
        x -= v1.x;
        y -= v1.y;
        return *this;
    }

    vec3 vec3::operator-(const vec3& v1) {
        x -= v1.x;
        y -= v1.y;
        z -= v1.z;
        return *this;
    }

    vec4 vec4::operator-(const vec4& v1) {
        x -= v1.x;
        y -= v1.y;
        z -= v1.z;
        w -= v1.w;
        return *this;
    }

    vec2 vec2::operator-(const Float& f) {
        x -= f;
        y -= f;
        return *this;
    }

    vec3 vec3::operator-(const Float& f) {
        x -= f;
        y -= f;
        z -= f;
        return *this;
    }

    vec4 vec4::operator-(const Float& f) {
        x -= f;
        y -= f;
        z -= f;
        w -= f;
        return *this;
    }

    vec2 vec2::operator/(const vec2& v1) {
        x /= v1.x;
        y /= v1.y;
        return *this;
    }

    vec3 vec3::operator/(const vec3& v1) {
        x /= v1.x;
        y /= v1.y;
        z /= v1.z;
        return *this;
    }

    vec4 vec4::operator/(const vec4& v1) {
        x /= v1.x;
        y /= v1.y;
        z /= v1.z;
        w /= v1.w;
        return *this;
    }

    vec2 vec2::operator/(const Float& f) {
        x /= f;
        y /= f;
        return *this;
    }

    vec3 vec3::operator/(const Float& f) {
        x /= f;
        y /= f;
        z /= f;
        return *this;
    }

    vec4 vec4::operator/(const Float& f) {
        x /= f;
        y /= f;
        z /= f;
        w /= f;
        return *this;
    }

    Bool vec2::operator==(const vec2& v1){
        return (Bool)(
            x == v1.x &&
            y == v1.y
            );
    }

    Bool vec3::operator==(const vec3& v1) {
        return (Bool)(
            x == v1.x &&
            y == v1.y &&
            z == v1.z
            );
    }

    Bool vec4::operator==(const vec4& v1) {
        return (Bool)(
            x == v1.x &&
            y == v1.y &&
            z == v1.z &&
            w == v1.w
            );
    }

    Bool vec2::operator!=(const vec2& v1) {
        return (Bool)(!(*this == v1));
    }

    Bool vec3::operator!=(const vec3& v1) {
        return (Bool)(!(*this == v1));
    }

    Bool vec4::operator!=(const vec4& v1) {
        return (Bool)(!(*this == v1));
    }

}
