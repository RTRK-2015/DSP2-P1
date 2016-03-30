#include "processing.h"
#include "iir.h"
#include <math.h>

#define FLT 32767

void calculateShelvingCoeff(float alpha, Int16* output)
{
	Int16 a = FLT * alpha;

	output[0] = a;
	output[1] = -1 * FLT;
	output[2] = 1*FLT - 1;
	output[3] = -a;

}

void calculatePeekCoeff(float alpha, float beta, Int16* output)
{
	Int16 a = FLT * alpha;
	Int16 b = FLT * beta;

	output[0] = a;
	output[1] = -b * (1 + a);
	output[2] = 1 * FLT;
	output[3] = 1 * FLT;
	output[4] = -b * (1 + a);
	output[5] = a;

}

void shelvingHP(Int16* input, Int16* coeff, Int16* z_x, Int16* z_y, Int16 n, Int16 k, Int16* output)
{
	int i;

	for (i = 0; i < n; i++)
	{
		Int16 tmp = first_order_IIR(input[i], coeff, z_x, z_y);
		output[i] = ((input[i] - tmp) >> 1) + ((input[i] + tmp) >> (1 - k));
	}
}

void shelvingLP(Int16* input, Int16* coeff, Int16* z_x, Int16* z_y, Int16 n, Int16 k, Int16* output)
{
	int i;

	for (i = 0; i < n; i++)
	{
		Int16 tmp = first_order_IIR(input[i], coeff, z_x, z_y);
		output[i] = ((input[i] - tmp) >> (1 - k)) + ((input[i] + tmp) >> 1);
	}
}

void shelvingPeek(Int16* input, Int16* coeff, Int16* z_x, Int16* z_y, Int16 n, Int16 k, Int16* output)
{
	int i;

	for (i = 0; i < n; i++)
	{
		Int16 tmp = second_order_IIR(input[i], coeff, z_x, z_y);
		output[i] = ((input[i] + tmp) >> 1) + ((input[i] - tmp) >> (1 - k));
	}
}

void equalize(Int16* input, int kBass, int kTreble, int kMid1, int kMid2, Int16* output)
{

}
