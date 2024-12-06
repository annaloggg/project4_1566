#include "myLib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

// Settings
int logging = 0;

void set_logging(int l)
{
    logging = l;
}

// Matrices
mat4 identity = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}};

// Logging
void print_v(vec4 v)
{
    printf("[ %7.3f %7.3f %7.3f %7.3f ]\n", v.x, v.y, v.z, v.w);
}

void print_v_nn(vec4 v)
{
    printf("[ %7.3f %7.3f %7.3f %7.3f ]", v.x, v.y, v.z, v.w);
}

void print_v2(vec2 v)
{
    printf("[ %7.3f %7.3f]\n", v.x, v.y);
}

void print_v2int(vec2int v)
{
    printf("[%d, %d]\n", v.x, v.y);
}

void print_v2int_nn(vec2int v)
{
    printf("[%d, %d]", v.x, v.y);
}

void printCoordinates(Coordinates v)
{
    printf("[ %d %d]\n", v.start, v.end);
}

void print_m3(mat3 m)
{
    printf(
        "[ %7.3f %7.3f %7.3f ]\n[ %7.3f %7.3f %7.3f ]\n[ %7.3f %7.3f %7.3f ]\n",
        m.x.x, m.y.x, m.z.x,
        m.x.y, m.y.y, m.z.y,
        m.x.z, m.y.z, m.z.z);
}

void print_m4(mat4 m)
{
    printf(
        "[ %7.3f %7.3f %7.3f %7.3f ]\n[ %7.3f %7.3f %7.3f %7.3f ]\n[ %7.3f %7.3f %7.3f %7.3f ]\n[ %7.3f %7.3f %7.3f %7.3f ]\n",
        m.x.x, m.y.x, m.z.x, m.w.x,
        m.x.y, m.y.y, m.z.y, m.w.y,
        m.x.z, m.y.z, m.z.z, m.w.z,
        m.x.w, m.y.w, m.z.w, m.w.w);
}

// Vector Operations
int v_eq(vec4 v1, vec4 v2)
{
    return (mag(sub_v(v1, v2)) < 0.001f);
}

int v2i_eq(vec2int v1, vec2int v2)
{
    // printf("v1: (%d, %d). v2: (%d, %d).\n", v1.x, v1.y, v2.x, v2.y);
    // printf("x equal? %d\n", (v1.x == v2.x));
    // printf("y equal? %d\n", (v1.y == v2.y));
    int result = ((v1.x == v2.x) && (v1.y == v2.y));
    // printf("Returning %d\n", result);
    return result;
}

vec4 add_v(vec4 v1, vec4 v2)
{
    vec4 res = {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z,
        v1.w + v2.w,
    };
    return res;
}

vec4 sub_v(vec4 v1, vec4 v2)
{
    vec4 res = {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z,
        v1.w - v2.w,
    };
    return res;
}

float mag(vec4 v)
{
    return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2) + pow(v.w, 2));
}

vec4 norm(vec4 v)
{
    return sv_mult(1 / mag(v), v);
}

float dot(vec4 v1, vec4 v2)
{
    return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w));
}

vec4 cross(vec4 v1, vec4 v2)
{
    vec4 res = {
        ((v1.y * v2.z) - (v1.z * v2.y)),
        ((v1.z * v2.x) - (v1.x * v2.z)),
        ((v1.x * v2.y) - (v1.y * v2.x)),
        0};
    return res;
}

// Matrix Operations
int m_eq(mat4 m1, mat4 m2)
{
    return (
        m1.x.x == m2.x.x &&
        m1.x.y == m2.x.y &&
        m1.x.z == m2.x.z &&
        m1.x.w == m2.x.w &&
        m1.y.x == m2.y.x &&
        m1.y.y == m2.y.y &&
        m1.y.z == m2.y.z &&
        m1.y.w == m2.y.w &&
        m1.z.x == m2.z.x &&
        m1.z.y == m2.z.y &&
        m1.z.z == m2.z.z &&
        m1.z.w == m2.z.w &&
        m1.w.x == m2.w.x &&
        m1.w.y == m2.w.y &&
        m1.w.z == m2.w.z &&
        m1.w.w == m2.w.w);
}

mat4 add_m(mat4 m1, mat4 m2)
{
    mat4 res = {
        {m1.x.x + m2.x.x, m1.x.y + m2.x.y, m1.x.z + m2.x.z, m1.x.w + m2.x.w},
        {m1.y.x + m2.y.x, m1.y.y + m2.y.y, m1.y.z + m2.y.z, m1.y.w + m2.y.w},
        {m1.z.x + m2.z.x, m1.z.y + m2.z.y, m1.z.z + m2.z.z, m1.z.w + m2.z.w},
        {m1.w.x + m2.w.x, m1.w.y + m2.w.y, m1.w.z + m2.w.z, m1.w.w + m2.w.w}};
    return res;
};

mat4 sub_m(mat4 m1, mat4 m2)
{
    mat4 res = {
        {m1.x.x - m2.x.x, m1.x.y - m2.x.y, m1.x.z - m2.x.z, m1.x.w - m2.x.w},
        {m1.y.x - m2.y.x, m1.y.y - m2.y.y, m1.y.z - m2.y.z, m1.y.w - m2.y.w},
        {m1.z.x - m2.z.x, m1.z.y - m2.z.y, m1.z.z - m2.z.z, m1.z.w - m2.z.w},
        {m1.w.x - m2.w.x, m1.w.y - m2.w.y, m1.w.z - m2.w.z, m1.w.w - m2.w.w}};
    return res;
};

// Matrix Transformations
mat4 m_cofactor(mat4 m)
{
    mat4 res = {
        {m.x.x, -m.x.y, m.x.z, -m.x.w},
        {-m.y.x, m.y.y, -m.y.z, m.y.w},
        {m.z.x, -m.z.y, m.z.z, -m.z.w},
        {-m.w.x, m.w.y, -m.w.z, m.w.w}};

    if (logging)
    {
        printf("Cofactor of: \n");
        print_m4(m);
        printf("is\n");
        print_m4(res);
        printf("\n");
    }

    return res;
}

mat4 m_transpose(mat4 m)
{
    mat4 res = {
        {m.x.x, m.y.x, m.z.x, m.w.x},
        {m.x.y, m.y.y, m.z.y, m.w.y},
        {m.x.z, m.y.z, m.z.z, m.w.z},
        {m.x.w, m.y.w, m.z.w, m.w.w}};

    if (logging)
    {
        printf("Transposition of: \n");
        print_m4(m);
        printf("is\n");
        print_m4(res);
        printf("\n");
    }

    return res;
};

float m3_det(mat3 m)
{
    float res = ((m.x.x * m.y.y * m.z.z) +
                 (m.y.x * m.z.y * m.x.z) +
                 (m.z.x * m.x.y * m.y.z) -
                 (m.x.z * m.y.y * m.z.x) -
                 (m.y.z * m.z.y * m.x.x) -
                 (m.z.z * m.x.y * m.y.x));

    if (logging)
    {
        printf("Determinant of \n");
        print_m3(m);
        printf("is %f\n\n", res);
    }

    return res;
}

float m4_det(mat4 m)
{
    float res = ((m.x.x * m.y.y * m.z.z * m.w.w) + (m.y.x * m.z.y * m.w.z * m.x.w) + (m.z.x * m.w.y * m.x.z * m.y.w) + (m.w.x * m.x.y * m.y.z * m.z.w) - (m.x.w * m.y.z * m.z.y * m.w.x) - (m.y.w * m.z.z * m.w.y * m.x.x) - (m.z.w * m.w.z * m.x.y * m.y.x) - (m.w.w * m.x.z * m.y.y * m.z.x));

    if (logging)
    {
        printf("Determinant of \n");
        print_m4(m);
        printf("is %f\n\n", res);
    }

    return res;
}

mat4 m_minor(mat4 m)
{

    mat3 mxx = {
        {m.y.y, m.y.z, m.y.w},
        {m.z.y, m.z.z, m.z.w},
        {m.w.y, m.w.z, m.w.w}};
    float det_xx = m3_det(mxx);

    mat3 mxy = {
        {m.y.x, m.y.z, m.y.w},
        {m.z.x, m.z.z, m.z.w},
        {m.w.x, m.w.z, m.w.w}};
    float det_xy = m3_det(mxy);

    mat3 mxz = {
        {m.y.x, m.y.y, m.y.w},
        {m.z.x, m.z.y, m.z.w},
        {m.w.x, m.w.y, m.w.w}};
    float det_xz = m3_det(mxz);

    mat3 mxw = {
        {m.y.x, m.y.y, m.y.z},
        {m.z.x, m.z.y, m.z.z},
        {m.w.x, m.w.y, m.w.z}};
    float det_xw = m3_det(mxw);

    mat3 myx = {
        {m.x.y, m.x.z, m.x.w},
        {m.z.y, m.z.z, m.z.w},
        {m.w.y, m.w.z, m.w.w}};
    float det_yx = m3_det(myx);

    mat3 myy = {
        {m.x.x, m.x.z, m.x.w},
        {m.z.x, m.z.z, m.z.w},
        {m.w.x, m.w.z, m.w.w}};
    float det_yy = m3_det(myy);

    mat3 myz = {
        {m.x.x, m.x.y, m.x.w},
        {m.z.x, m.z.y, m.z.w},
        {m.w.x, m.w.y, m.w.w}};
    float det_yz = m3_det(myz);

    mat3 myw = {
        {m.x.x, m.x.y, m.x.z},
        {m.z.x, m.z.y, m.z.z},
        {m.w.x, m.w.y, m.w.z}};
    float det_yw = m3_det(myw);

    mat3 mzx = {
        {m.x.y, m.x.z, m.x.w},
        {m.y.y, m.y.z, m.y.w},
        {m.w.y, m.w.z, m.w.w}};
    float det_zx = m3_det(mzx);

    mat3 mzy = {
        {m.x.x, m.x.z, m.x.w},
        {m.y.x, m.y.z, m.y.w},
        {m.w.x, m.w.z, m.w.w}};
    float det_zy = m3_det(mzy);

    mat3 mzz = {
        {m.x.x, m.x.y, m.x.w},
        {m.y.x, m.y.y, m.y.w},
        {m.w.x, m.w.y, m.w.w}};
    float det_zz = m3_det(mzz);

    mat3 mzw = {
        {m.x.x, m.x.y, m.x.z},
        {m.y.x, m.y.y, m.y.z},
        {m.w.x, m.w.y, m.w.z}};
    float det_zw = m3_det(mzw);

    mat3 mwx = {
        {m.x.y, m.x.z, m.x.w},
        {m.y.y, m.y.z, m.y.w},
        {m.z.y, m.z.z, m.z.w}};
    float det_wx = m3_det(mwx);

    mat3 mwy = {
        {m.x.x, m.x.z, m.x.w},
        {m.y.x, m.y.z, m.y.w},
        {m.z.x, m.z.z, m.z.w}};
    float det_wy = m3_det(mwy);

    mat3 mwz = {
        {m.x.x, m.x.y, m.x.w},
        {m.y.x, m.y.y, m.y.w},
        {m.z.x, m.z.y, m.z.w}};
    float det_wz = m3_det(mwz);

    mat3 mww = {
        {m.x.x, m.x.y, m.x.z},
        {m.y.x, m.y.y, m.y.z},
        {m.z.x, m.z.y, m.z.z}};
    float det_ww = m3_det(mww);

    mat4 res = {
        {m3_det(mxx), m3_det(mxy), m3_det(mxz), m3_det(mxw)},
        {m3_det(myx), m3_det(myy), m3_det(myz), m3_det(myw)},
        {m3_det(mzx), m3_det(mzy), m3_det(mzz), m3_det(mzw)},
        {m3_det(mwx), m3_det(mwy), m3_det(mwz), m3_det(mww)}};

    if (logging)
    {
        printf("Determinant of mxx: %f\n", det_xx);
        printf("Determinant of mxy: %f\n", det_xy);
        printf("Determinant of mxz: %f\n", det_xz);
        printf("Determinant of mxw: %f\n", det_xw);
        printf("Determinant of myx: %f\n", det_yx);
        printf("Determinant of myy: %f\n", det_yy);
        printf("Determinant of myz: %f\n", det_yz);
        printf("Determinant of myw: %f\n", det_yw);
        printf("Determinant of mzx: %f\n", det_zx);
        printf("Determinant of mzy: %f\n", det_zy);
        printf("Determinant of mzz: %f\n", det_zz);
        printf("Determinant of mzw: %f\n", det_zw);
        printf("Determinant of mwx: %f\n", det_wx);
        printf("Determinant of mwy: %f\n", det_wy);
        printf("Determinant of mwz: %f\n", det_wz);
        printf("Determinant of mww: %f\n", det_ww);

        printf("Matrix of Minor: \n");
        print_m4(res);
    }

    return res;
}

mat4 m_inverse(mat4 m)
{
    mat4 res = m;
    res = m_minor(res);
    res = m_cofactor(res);
    res = m_transpose(res);
    float det = m4_det(m);
    res = sm_mult(1.0f / m4_det(m), res); // TODO: check that det(m) isn't 0 !

    if (logging)
    {
        printf("Inversion of \n");
        print_m4(m);
        printf("is\n");
        print_m4(res);
        printf("\n");
    }

    // if (m_eq(mm_mult(m, res), identity)) // TODO: what to return if inverse doesn't exist ?
    return res;
};

/// Multiplication
vec4 sv_mult(float s, vec4 v)
{
    v.x *= s;
    v.y *= s;
    v.z *= s;
    v.w *= s;
    return v;
}

mat4 sm_mult(float s, mat4 m)
{
    mat4 res = {
        {m.x.x * s, m.x.y * s, m.x.z * s, m.x.w * s},
        {m.y.x * s, m.y.y * s, m.y.z * s, m.y.w * s},
        {m.z.x * s, m.z.y * s, m.z.z * s, m.z.w * s},
        {m.w.x * s, m.w.y * s, m.w.z * s, m.w.w * s}};
    return res;
};

vec4 mv_mult(mat4 m, vec4 v)
{
    vec4 res = {
        (m.x.x * v.x) + (m.y.x * v.y) + (m.z.x * v.z) + (m.w.x * v.w),
        (m.x.y * v.x) + (m.y.y * v.y) + (m.z.y * v.z) + (m.w.y * v.w),
        (m.x.z * v.x) + (m.y.z * v.y) + (m.z.z * v.z) + (m.w.z * v.w),
        (m.x.w * v.x) + (m.y.w * v.y) + (m.z.w * v.z) + (m.w.w * v.w)};
    return res;
}

mat4 mm_mult(mat4 m1, mat4 m2)
{
    mat4 res = {
        {
            (m1.x.x * m2.x.x) + (m1.y.x * m2.x.y) + (m1.z.x * m2.x.z) + (m1.w.x * m2.x.w),
            (m1.x.y * m2.x.x) + (m1.y.y * m2.x.y) + (m1.z.y * m2.x.z) + (m1.w.y * m2.x.w),
            (m1.x.z * m2.x.x) + (m1.y.z * m2.x.y) + (m1.z.z * m2.x.z) + (m1.w.z * m2.x.w),
            (m1.x.w * m2.x.x) + (m1.y.w * m2.x.y) + (m1.z.w * m2.x.z) + (m1.w.w * m2.x.w),
        },
        {
            (m1.x.x * m2.y.x) + (m1.y.x * m2.y.y) + (m1.z.x * m2.y.z) + (m1.w.x * m2.y.w),
            (m1.x.y * m2.y.x) + (m1.y.y * m2.y.y) + (m1.z.y * m2.y.z) + (m1.w.y * m2.y.w),
            (m1.x.z * m2.y.x) + (m1.y.z * m2.y.y) + (m1.z.z * m2.y.z) + (m1.w.z * m2.y.w),
            (m1.x.w * m2.y.x) + (m1.y.w * m2.y.y) + (m1.z.w * m2.y.z) + (m1.w.w * m2.y.w),
        },
        {
            (m1.x.x * m2.z.x) + (m1.y.x * m2.z.y) + (m1.z.x * m2.z.z) + (m1.w.x * m2.z.w),
            (m1.x.y * m2.z.x) + (m1.y.y * m2.z.y) + (m1.z.y * m2.z.z) + (m1.w.y * m2.z.w),
            (m1.x.z * m2.z.x) + (m1.y.z * m2.z.y) + (m1.z.z * m2.z.z) + (m1.w.z * m2.z.w),
            (m1.x.w * m2.z.x) + (m1.y.w * m2.z.y) + (m1.z.w * m2.z.z) + (m1.w.w * m2.z.w),
        },
        {
            (m1.x.x * m2.w.x) + (m1.y.x * m2.w.y) + (m1.z.x * m2.w.z) + (m1.w.x * m2.w.w),
            (m1.x.y * m2.w.x) + (m1.y.y * m2.w.y) + (m1.z.y * m2.w.z) + (m1.w.y * m2.w.w),
            (m1.x.z * m2.w.x) + (m1.y.z * m2.w.y) + (m1.z.z * m2.w.z) + (m1.w.z * m2.w.w),
            (m1.x.w * m2.w.x) + (m1.y.w * m2.w.y) + (m1.z.w * m2.w.z) + (m1.w.w * m2.w.w),
        }};
    return res;
};

/// Transformations
vec4 negate(vec4 in)
{
    return (vec4){-in.x, -in.y, -in.z, in.z};
}

vec2 negate_v2(vec2 in)
{
    return (vec2){-in.x, -in.y};
}

mat4 translate_mat4(vec4 dist)
{
    return (mat4){
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {dist.x, dist.y, dist.z, 1.0},
    };
}

vec4 translate(vec4 in, vec4 dist)
{
    return mv_mult(translate_mat4(dist), in);
}

mat4 scale_uniform_mat4(float scale)
{
    return (mat4){
        {scale, 0.0, 0.0, 0.0},
        {0.0, scale, 0.0, 0.0},
        {0.0, 0.0, scale, 0.0},
        {0.0, 0.0, 0.0, 1.0},
    };
}

mat4 scale_mat4(vec4 scale)
{
    return (mat4){
        {scale.x, 0.0, 0.0, 0.0},
        {0.0, scale.y, 0.0, 0.0},
        {0.0, 0.0, scale.z, 0.0},
        {0.0, 0.0, 0.0, 1.0},
    };
}

vec4 scale(vec4 in, vec4 scale, vec4 ref)
{

    vec4 res = translate(in, negate(ref));

    res = mv_mult(scale_mat4(scale), res);

    return res = translate(res, ref);
}

float deg(float theta)
{
    return theta * 180 / M_PI;
}

float rad(float theta)
{
    return theta * M_PI / 180.0;
}

mat4 rotate_x_mat4(float theta)
{
    theta = rad(theta);
    return (mat4){
        {1.0, 0.0, 0.0, 0.0},
        {0.0, cos(theta), -sin(theta), 0.0},
        {0.0, sin(theta), cos(theta), 0.0},
        {0.0, 0.0, 0.0, 1.0},
    };
}

mat4 rotate_y_mat4(float theta)
{
    theta = rad(theta);
    return (mat4){
        {cos(theta), 0.0, sin(theta), 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {-sin(theta), 0.0, cos(theta), 0.0},
        {0.0, 0.0, 0.0, 1.0},
    };
}

mat4 rotate_z_mat4(float theta)
{
    theta = rad(theta);
    return (mat4){
        {cos(theta), sin(theta), 0.0, 0.0},
        {-sin(theta), cos(theta), 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0},
    };
}

vec4 rotate_x(vec4 in, vec4 ref, float theta)
{
    vec4 res = translate(in, negate(ref));
    res = mv_mult(rotate_x_mat4(theta), res);
    return res = translate(res, ref);
}

vec4 rotate_y(vec4 in, vec4 ref, float theta)
{
    vec4 res = translate(in, negate(ref));
    res = mv_mult(rotate_y_mat4(theta), res);
    return res = translate(res, ref);
}

vec4 rotate_z(vec4 in, vec4 ref, float theta)
{
    vec4 res = translate(in, negate(ref));
    res = mv_mult(rotate_z_mat4(theta), res);
    return res = translate(res, ref);
}

vec4 rotate_v(vec4 in, vec4 ref, vec4 axis, float theta)
{
    vec4 res = translate(in, negate(ref));
    res = mv_mult(rotate_v_mat4(axis, theta), res);
    return res = translate(res, ref);
}

mat4 rotate_v_mat4(vec4 axis, float theta)
{
    theta = deg(theta);
    float d = sqrt(pow(axis.y, 2) + pow(axis.z, 2));
    mat4 rx = (mat4){
        {1.0, 0.0, 0.0, 0.0},
        {0.0, axis.z / d, axis.y / d, 0.0},
        {0.0, -axis.y / d, axis.z / d, 0.0},
        {0.0, 0.0, 0.0, 1.0},
    };
    mat4 ry = (mat4){
        {d, 0.0, -axis.x, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {axis.x, 0.0, d, 0.0},
        {0.0, 0.0, 0.0, 1.0},
    };
    mat4 m1 = mm_mult(m_transpose(ry), rx);
    mat4 m2 = mm_mult(rotate_z_mat4(theta), m1);
    mat4 m3 = mm_mult(ry, m2);
    mat4 r = mm_mult(m_transpose(rx), m3);
    return r;
}

/// Helper
float min(float a, float b)
{
    if (b < a)
        return b;
    else
        return a;
}

float max(float a, float b)
{
    if (b > a)
        return b;
    else
        return a;
}

vec4 rand_color()
{
    return (vec4){
        (float)rand() / RAND_MAX,
        (float)rand() / RAND_MAX,
        (float)rand() / RAND_MAX};
}

vec4 center_tri(vec4 x, vec4 y, vec4 z)
{
    return (vec4){
        (x.x + y.x + z.x) / 3,
        (x.y + y.y + z.y) / 3,
        (x.z + y.z + z.z) / 3,
        1.0};
}

vec4 center_cube(float l, float r, float d, float u, float f, float b)
{

    // printf("Finding center of cube...\n");
    // printf("\tx: %f to %f\n", l, r);
    // printf("\ty: %f to %f\n", d, u);
    // printf("\tz: %f to %f\n", b, f);

    float x = ((r - l) / 2) + l;
    float y = ((u - d) / 2) + d;
    float z = ((f - b) / 2) + b;

    return (vec4){x, y, z, 1.0f};
}

int v_isnan(vec4 in)
{
    return (isnan(in.x) || isnan(in.y) || isnan(in.z) || isnan(in.w));
}

vec4 move_towards(vec4 from, vec4 to, float max)
{

    vec4 from_to = sub_v(to, from);

    if (mag(from_to) < max)
    {
        return to;
    }

    from_to = sv_mult(max, norm(from_to));
    vec4 res = add_v(from, from_to);

    return res;
}
