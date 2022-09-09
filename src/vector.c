#include <math.h>
#include "vector.h"

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