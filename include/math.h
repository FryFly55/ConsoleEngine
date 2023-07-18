//
// Created by FryFly55 on 17.06.2023.
//

#ifndef CONSOLEENGINE_MATH_H
#define CONSOLEENGINE_MATH_H

#pragma once

template<typename T>
struct vec2 {
    T x;
    T y;

    vec2 operator+(vec2 const& vec) {
        vec2 res;
        res.x = x + vec.x;
        res.y = y + vec.y;
        return res;
    }

    vec2 operator-(vec2 const& vec) {
        vec2 res;
        res.x = x - vec.x;
        res.y = y - vec.y;
        return res;
    }

    vec2 operator*(vec2 const& vec) {
        vec2 res;
        res.x = x * vec.x;
        res.y = y * vec.y;
        return res;
    }

    template<typename Z>
    vec2 operator*(Z const& a) {
        vec2 res;
        res.x = x * a;
        res.y = y * a;
        return res;
    }

    vec2 operator/(vec2 const& vec) {
        vec2 res;
        res.x = x / vec.x;
        res.y = y / vec.y;
        return res;
    }

    template<typename Z>
    vec2 operator/(Z const& a) {
        vec2 res;
        res.x = x / a;
        res.y = y / a;
        return res;
    }

    int scalarProd(vec2 const& vec) {
        vec2 res;
        res.x = x * vec.x;
        res.y = y * vec.y;
        return res.x + res.y;
    }
};

template<typename T>
struct vec3 {
    T x;
    T y;
    T z;

    vec3 operator+(vec3 const& vec) {
        vec3 res;
        res.x = x + vec.x;
        res.y = y + vec.y;
        res.z = z + vec.z;
        return res;
    }

    vec3 operator-(vec3 const& vec) {
        vec3 res;
        res.x = x - vec.x;
        res.y = y - vec.y;
        res.z = z - vec.z;
        return res;
    }

    vec3 operator*(vec3 const& vec) {
        vec3 res;
        res.x = x * vec.x;
        res.y = y * vec.y;
        res.z = z * vec.z;
        return res;
    }

    template<typename Z>
    vec3 operator*(Z const& a) {
        vec3 res;
        res.x = x * a;
        res.y = y * a;
        res.z = z * a;
        return res;
    }

    vec3 operator/(vec3 const& vec) {
        vec3 res;
        res.x = x / vec.x;
        res.y = y / vec.y;
        res.z = z / vec.z;
        return res;
    }

    template<typename Z>
    vec3 operator/(Z const& a) {
        vec3 res;
        res.x = x / a;
        res.y = y / a;
        res.z = z / a;
        return res;
    }
    int scalarProd(vec3 const& vec) {
        vec3 res;
        res.x = x * vec.x;
        res.y = y * vec.y;
        res.z = z * vec.z;
        return res.x + res.y + res.z;
    }
};

template<typename T>
struct vec4 {
    T x;
    T y;
    T z;
    T w;

    vec4 operator+(vec4 const& vec) {
        vec4 res;
        res.x = x + vec.x;
        res.y = y + vec.y;
        res.z = z + vec.z;
        res.w = w + vec.w;
        return res;
    }

    vec4 operator-(vec4 const& vec) {
        vec4 res;
        res.x = x - vec.x;
        res.y = y - vec.y;
        res.z = z - vec.z;
        res.w = w - vec.w;
        return res;
    }

    vec4 operator*(vec4 const& vec) {
        vec4 res;
        res.x = x * vec.x;
        res.y = y * vec.y;
        res.z = z * vec.z;
        res.w = w * vec.w;
        return res;
    }

    template<typename Z>
    vec4 operator*(Z const& a) {
        vec4 res;
        res.x = x * a;
        res.y = y * a;
        res.z = z * a;
        res.w = w * a;
        return res;
    }

    vec4 operator/(vec4 const& vec) {
        vec4 res;
        res.x = x / vec.x;
        res.y = y / vec.y;
        res.z = z / vec.z;
        res.w = w / vec.w;
        return res;
    }

    template<typename Z>
    vec4 operator/(Z const& a) {
        vec4 res;
        res.x = x / a;
        res.y = y / a;
        res.z = z / a;
        res.w = w / a;
        return res;
    }

    int scalarProd(vec4 const& vec) {
        vec4 res;
        res.x = x * vec.x;
        res.y = y * vec.y;
        res.z = z * vec.z;
        res.w = w * vec.w;
        return res.x + res.y + res.z + res.w;
    }
};

template <typename T>
struct mat2 {
    vec2<T> vec0;
    vec2<T> vec1;
};

template <typename T>
struct mat3 {
    vec3<T> vec0;
    vec3<T> vec1;
    vec3<T> vec2;
};

template <typename T>
struct mat4 {
    vec4<T> vec0;
    vec4<T> vec1;
    vec4<T> vec2;
    vec4<T> vec3;
};

#endif //CONSOLEENGINE_MATH_H
