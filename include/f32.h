#ifndef F32_H
#define F32_H

#include "Types.h"

#define SHIFT_AMT_F32 16
#define SHIFT_MSK_F32 ((1 << SHIFT_AMT_F32) - 1) // 65535

typedef I32 F32;

F32 create_f32(I32 mag, U32 frac)
{
	return (mag << SHIFT_AMT_F32) + frac;
}

F32 add_f32(F32 a, F32 b)
{
	return a + b;
}

F32 sub_f32(F32 a, F32 b)
{
	return a - b;
}

F32 mul_f32(F32 a, F32 b)
{
	I64 r = (I64)a * (I64)b;
	return r >> SHIFT_AMT_F32;
}

F32 div_f32(F32 a, F32 b)
{
	I64 x = (I64)a << SHIFT_AMT_F32;
	return x / b;
}

I16 mag_f32(F32 f)
{
	return f >> SHIFT_AMT_F32;
}

I16 frac_f32(F32 f)
{
	return f & SHIFT_MSK_F32;
}

#ifdef FLOAT_ARITHMETIC

double float_f32(F32 f)
{
	return (double)(f >> SHIFT_AMT_F32) + 
			(double)(f & SHIFT_MSK_F32) / (1 << SHIFT_AMT_F32);
}

#else

F32 float_f32(F32 f)
{
	return f;
}

#endif

#endif // F32_H