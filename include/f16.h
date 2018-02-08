#ifndef F16_H
#define F16_H

#include "Types.h"

#define SHIFT_AMT_F16 8
#define SHIFT_MSK_F16 ((1 << SHIFT_AMT_F16) - 1)

typedef I16 F16;

F16 create_f16(I16 mag, U16 frac)
{
	return (mag << SHIFT_AMT_F16) + frac;
}

F16 create_f16f(double f)
{
	return (I16)(f * (1 << SHIFT_AMT_F16));
}

F16 add_f16(F16 a, F16 b)
{
	return a + b;
}

F16 sub_f16(F16 a, F16 b)
{
	return a - b;
}

F16 mul_f16(F16 a, F16 b)
{
	I32 r = (I32)a * (I32)b;
	return r >> SHIFT_AMT_F16;
}

F16 div_f16(F16 a, F16 b)
{
	I32 x = (I32)a << SHIFT_AMT_F16;
	return x / b;
}

I16 mag_f16(F16 f)
{
	return f >> SHIFT_AMT_F16;
}

I16 frac_f16(F16 f)
{
	return f & SHIFT_MSK_F16;
}

#ifdef FLOAT_ARITHMETIC

double float_f16(F16 f)
{
	return (double)(f >> SHIFT_AMT_F16) + 
			(double)(f & SHIFT_MSK_F16) / (1 << SHIFT_AMT_F16);
}

#else

F16 float_f16(F16 f)
{
	return f;
}

#endif

#endif // F16_H