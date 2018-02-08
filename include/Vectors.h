#ifndef VECTORS_H
#define VECTORS_H

#include "Types.h"
#include "f16.h"
#include <math.h>

#define VEC_ADD

typedef struct Vec2
{
	F16 x, y;
} Vec2;

typedef struct Vec3
{
	F16 x, y, z;
} Vec3;

Vec2 unit_vec2()
{
	return { .x=create_f16(1, 0), .y=create_f16(1, 0) };
}

Vec3 unit_vec3()
{
	return { .x=create_f16(1, 0),
		.y=create_f16(1, 0),
		.z=create_f16(1, 0)};
}

Vec2 negate_vec2(Vec2 v)
{
	return { .x=-v.x, .y=-v.y };
}

Vec3 negate_vec3(Vec3 v)
{
	return { .x=-v.x, .y=-v.y, .z=-v.z };
}

Vec2 create_vec2f(F16 x, F16 y)
{
	return { .x=x, .y=y };
}

Vec3 create_vec3f(F16 x, F16 y, F16 z)
{
	return { .x=x, .y=y, .z=z };
}

Vec2 create_vec2i(I16 x, I16 y)
{
	return { .x=create_f16(x, 0), .y=create_f16(y, 0) };
}

Vec3 create_vec3i(I16 x, I16 y, I16 z)
{
	return { .x=create_f16(x, 0), .y=create_f16(y, 0), .z=create_f16(z, 0) };
}

Vec2 add_vec2(Vec2 a, Vec2 b)
{
	return { .x=a.x + b.x, .y=a.y + b.y };
}

Vec3 add_vec3(Vec3 a, Vec3 b)
{
	return { .x=a.x + b.x, .y=a.y + b.y, .z=a.z + b.z };
}

Vec2 sub_vec2(Vec2 a, Vec2 b)
{
	return add_vec2(a, {.x=-b.x, .y=-b.y});
}

Vec3 sub_vec3(Vec3 a, Vec3 b)
{
	return add_vec3(a, {.x=-b.x, .y=-b.y, .z=-b.z});
}

Vec2 mul_vec2(Vec2 a, F16 b)
{
	return {.x=mul_f16(a.x, b), .y=mul_f16(a.y, b)};
}

Vec3 mul_vec3(Vec3 a, F16 b)
{
	return {.x=mul_f16(a.x, b), .y=mul_f16(a.y, b), .z=mul_f16(a.z, b)};
}

Vec2 div_vec2(Vec2 a, F16 b)
{
	return {.x=div_f16(a.x, b), .y=div_f16(a.y, b)};
}

Vec3 div_vec3(Vec3 a, F16 b)
{
	return {.x=div_f16(a.x, b), .y=div_f16(a.y, b), .z=div_f16(a.z, b)};
}

F16 dot_vec2(Vec2 a, Vec2 b)
{
	return add_f16(add_f16(a.x, b.x), add_f16(a.y, b.y));
}

F16 dot_vec3(Vec3 a, Vec3 b)
{
	return add_f16(
			add_f16(
				add_f16(a.x, b.x), 
				add_f16(a.y, b.y)
			), 
			add_f16(a.z, b.z)
		);
}

Vec3 cross_vec3(Vec3 a, Vec3 b)
{
	Vec3 result;
	result.x = sub_f16(mul_f16(a.y, b.z), mul_f16(a.z, b.y));
	result.y = sub_f16(mul_f16(a.z, b.x), mul_f16(a.x, b.z));
	result.z = sub_f16(mul_f16(a.x, b.y), mul_f16(a.y, b.x));
}

F16 length2_vec2(Vec2 v)
{
	return add_f16(
		mul_f16(v.x, v.x),
		mul_f16(v.y, v.y)
	);
}

F16 length2_vec3(Vec3 v)
{
	return add_f16(
		add_f16(
			mul_f16(v.x, v.x),
			mul_f16(v.y, v.y)
		),
		mul_f16(v.z, v.z)
	);
}

F16 length_vec2(Vec2 v)
{
	return create_f16f(sqrt(float_f16(length2_vec2(v))));
}

F16 length_vec3(Vec3 v)
{
	return create_f16f(sqrt(float_f16(length2_vec3(v))));
}

Vec2 normalize_vec2(Vec2 v)
{
	F16 length = length_vec2(v);
	return div_vec2(v, length);
}

Vec3 normalize_vec3(Vec3 v)
{
	F16 length = length_vec3(v);
	return div_vec3(v, length);
}

#endif // VECTORS_H