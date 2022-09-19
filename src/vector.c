#include <math.h>
#include "vector.h"

float vec2_length(vec2_t v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

vec2_t vec2_add(vec2_t a, vec2_t b) {
    vec2_t result = {
        .x = a.x + b.x,
        .y = a.y + b.y
    };
    return result;
}
vec2_t vec2_sub(vec2_t a, vec2_t b) {
    vec2_t result = {
        .x = a.x - b.x,
        .y = a.y - b.y
    };
    return result;
}

vec2_t vec2_mul(vec2_t v, float factor) {
    vec2_t result = {
        .x = v.x * factor,
        .y = v.y * factor
    };
    return result;
}

vec2_t vec2_div(vec2_t v, float factor) {
    vec2_t result = {
        .x = v.x / factor,
        .y = v.y / factor
    };
    return result;
}

float vec2_dot(vec2_t a, vec2_t b) {
    return a.x * b.x + a.y * b.y;
}

void vec2_normalize(vec2_t *v) {
    float length = vec2_length(*v);
    v->x /= length;
    v->y /= length;
}

float vec3_length(vec3_t v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
    vec3_t result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}
vec3_t vec3_sub(vec3_t a, vec3_t b) {
    vec3_t result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

vec3_t vec3_mul(vec3_t v, float factor) {
    vec3_t result;
    result.x = v.x * factor;
    result.y = v.y * factor;
    result.z = v.z * factor;
    return result;
}

vec3_t vec3_div(vec3_t v, float factor) {
    vec3_t result;
    result.x = v.x / factor;
    result.y = v.y / factor;
    result.z = v.z / factor;
    return result;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
    vec3_t result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

float vec3_dot(vec3_t a, vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

void vec3_normalize(vec3_t *v) {
    float length = vec3_length(*v);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

vec3_t vec3_rotate_x(vec3_t v, float angle) {
    vec3_t result;
    result.x = v.x;
    result.y = v.y * cos(angle) - v.z * sin(angle);
    result.z = v.y * sin(angle) + v.z * cos(angle);
    return result;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
    vec3_t result;
    result.x = v.x * cos(angle) - v.z * sin(angle);
    result.y = v.y;
    result.z = v.x * sin(angle) + v.z * cos(angle);
    return result;
}

vec3_t vec3_rotate_z(vec3_t v, float angle) {
    vec3_t result;
    result.x = v.x * cos(angle) - v.y * sin(angle);
    result.y = v.x * sin(angle) + v.y * cos(angle);
    result.z = v.z;
    return result;
}