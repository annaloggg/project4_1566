#ifndef _MYLIB_H_

#define _MYLIB_H_

#define RED {1.0, 0.0, 0.0, 1.0}
#define GREEN {0.0, 1.0, 0.0, 1.0}
#define BLUE {0.0, 0.0, 1.0, 1.0}
#define SQUARES 36

#define ORIGIN \
    (vec4) { 0.0, 0.0, 0.0, 1.0 }
#define ONE \
    (vec4) { 1.0, 1.0, 1.0, 0.0 }
#define ZERO \
    (vec4) { 0.0, 0.0, 0.0, 0.0 }
#define LEFT \
    (vec4) { -1.0, 0.0, 0.0, 0.0 }
#define RIGHT \
    (vec4) { 1.0, 0.0, 0.0, 0.0 }
#define UP \
    (vec4) { 0.0, 1.0, 0.0, 0.0 }
#define DOWN \
    (vec4) { 0.0, -1.0, 0.0, 0.0 }
#define FORWARD \
    (vec4) { 0.0, 0.0, 1.0, 0.0 }
#define NORTH \
    (vec2int) { 0, 1 }
#define SOUTH \
    (vec2int) { 0, -1 }
#define EAST \
    (vec2int) { 1, 0 }
#define WEST \
    (vec2int) { -1, 0 }
#define IDENTITY                                                 \
    (mat4)                                                       \
    {                                                            \
        {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, { 0, 0, 0, 1 } \
    }

#define INITIAL_CAPACITY 25

#ifndef M_PI
#define M_PI 3.14159265359
#endif

typedef struct
{
    float x;
    float y;
} vec2;

typedef struct
{
    int x;
    int y;
} vec2int;

typedef struct
{
    float x;
    float y;
    float z;
} vec3;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} vec4;

typedef struct
{
    vec4 x;
    vec4 y;
    vec4 z;
    vec4 w;
} mat4;

typedef struct
{
    vec3 x;
    vec3 y;
    vec3 z;
} mat3;

typedef struct Node
{
    vec2 parent_coordinates; // node from
    vec2 coordinates;        // maze[x][y] position
    int g_cost;              // distance from starting node
    int h_cost;              // distance from ending node
    int f_cost;              // g_cost + h_cost (total distance)
} Node;

typedef struct
{
    Node *array;
    int size;     // Current number of elements
    int capacity; // Total capacity
} NodeMinHeap;

typedef struct
{
    int start;
    int end;
} Coordinates;

typedef struct
{
    Coordinates *array;
    int size;
    int capacity;
    int index;
} ArrayList;

/// Logging
void print_v(vec4);
void print_v_nn(vec4);
void print_v2(vec2 v);
void print_v2int(vec2int v);
void print_v2int_nn(vec2int v);
void print_m4(mat4);
void print_m3(mat3);

/// Vector Operations
int v_eq(vec4, vec4);
int v2i_eq(vec2int, vec2int);

// Addition
vec4 add_v(vec4, vec4);
vec4 sub_v(vec4, vec4);

// Magnitude
float mag(vec4);
vec4 norm(vec4);

// Multiplication
float dot(vec4, vec4);
vec4 cross(vec4, vec4);

/// Matrix Operations
int m_eq(mat4, mat4);
mat4 add_m(mat4, mat4);
mat4 sub_m(mat4, mat4);

// Transformation
mat4 m_cofactor(mat4);
mat4 m_transpose(mat4);

// Inversion
float m3_det(mat3);
float m4_det(mat4);
mat4 m_minor(mat4);
mat4 m_inverse(mat4);

/// Multiplication
vec4 sv_mult(float, vec4);
mat4 sm_mult(float, mat4);
vec4 mv_mult(mat4, vec4);
mat4 mm_mult(mat4, mat4);

/// Transformations
vec4 negate(vec4);
vec2 negate_v2(vec2);
mat4 translate_mat4(vec4);
vec4 translate(vec4, vec4);
mat4 scale_mat4(vec4);
mat4 scale_uniform_mat4(float);
vec4 scale(vec4, vec4, vec4);
mat4 rotate_x_mat4(float);
mat4 rotate_y_mat4(float);
mat4 rotate_z_mat4(float);
mat4 rotate_v_mat4(vec4, float);
vec4 rotate_x(vec4, vec4, float);
vec4 rotate_y(vec4, vec4, float);
vec4 rotate_z(vec4, vec4, float);
vec4 rotate_v(vec4 in, vec4 ref, vec4 axis, float theta);

/// Helper
float max(float, float);
float min(float, float);
vec4 rand_color();
float deg(float);
vec4 center_tri(vec4, vec4, vec4);
vec4 center_cube(float, float, float, float, float, float);
float scale_cube(float, float, float, float, float, float);
int v_isnan(vec4);
vec4 move_towards(vec4 from, vec4 to, float max);
#endif
