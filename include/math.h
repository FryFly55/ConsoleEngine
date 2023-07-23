//
// Created by FryFly55 on 17.06.2023.
//

#ifndef CONSOLEENGINE_MATH_H
#define CONSOLEENGINE_MATH_H

#pragma once

const double PI = 3.14159265359;

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

    T scalarProd(vec2 const& vec) {
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
    T scalarProd(vec3 const& vec) {
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

    T scalarProd(vec4 const& vec) {
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
    T elm[2*2];

    template<typename Z>
    mat2 operator*(Z const& a) {
        mat2 res;
        for (int i = 0; i < 2*2; i++) {
            res[i] = elm[i] * a;
        }
        return res;
    }

    mat2 operator+(mat2 const& mat) {
        mat2 res;
        for (int i = 0; i < 2*2; i++) {
            res[i] = elm[i] + mat.elm[i];
        }
        return res;
    }

    mat2 operator-(mat2 const& mat) {
        mat2 res;
        for (int i = 0; i < 2*2; i++) {
            res[i] = elm[i] - mat.elm[i];
        }
        return res;
    }

    mat2 operator*(mat2 const& mat) {
        mat2 res =
                {
                        {elm[0] * elm[3] + mat.elm[0] * mat.elm[1]},
                        {elm[0] * elm[1] + mat.elm[1] * mat.elm[3]},
                        {elm[0] * elm[2] + mat.elm[2] * mat.elm[3]},
                        {elm[2] * elm[3] + mat.elm[3] * mat.elm[1]}
                };
        return res;
    }

    vec2<T> operator*(vec2<T> const& vec) {
        vec2<T> res =
                {
                        {elm[0] * vec.x + elm[1] * vec.y},
                        {elm[2] * vec.x + elm[3] * vec.y}
                };
        return res;
    }
};

template <typename T>
struct mat3 {
    vec3<T> column0;
    vec3<T> column;
    vec3<T> column2;
};

template <typename T>
struct mat4 {
    vec4<T> column0;
    vec4<T> column1;
    vec4<T> column2;
    vec4<T> column3;
};



#endif //CONSOLEENGINE_MATH_H
