#include "processing.h"
#include "iir.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FLT 32767


static Int16 coeff_lp[4];
static Int16 coeff_hp[4];
static Int16 coeff_m1[6];
static Int16 coeff_m2[6];


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
	output[1] = (-b * (1 + a)) >> 1;
	output[2] = 1 * FLT;

	output[3] = 1 * FLT;
	output[4] = (-b * (1 + a)) >> 1;
	output[5] = a;

}


void shelvingHP(Int16* input, Int16* coeff, Int16* z_x, Int16* z_y, Int16 n, Int16 k, Int16* output)
{
	int i;

	for (i = 0; i < n; i++)
	{
		Int16 tmp3 = input[i];
		Int16 tmp = first_order_IIR(tmp3, coeff, z_x, z_y);
		Int16 tmp2 = tmp3 + tmp;

		output[i] = ((tmp3 - tmp) >> 1)  +  (k > 0? tmp2 << (k - 1) : tmp2 >> (1 - k));
	}
}


void shelvingLP(Int16* input, Int16* coeff, Int16* z_x, Int16* z_y, Int16 n, Int16 k, Int16* output)
{
	int i;

	for (i = 0; i < n; i++)
	{
		Int16 tmp3 = input[i];
		Int16 tmp = first_order_IIR(tmp3, coeff, z_x, z_y);
		Int16 tmp2 = tmp3 - tmp;

		output[i] = (k > 0? tmp2 << (k - 1) : tmp2 >> (1 - k))  +  ((tmp3 + tmp) >> 1);
	}
}


void shelvingPeek(Int16* input, Int16* coeff, Int16* z_x, Int16* z_y, Int16 n, Int16 k, Int16* output)
{
	int i;

	for (i = 0; i < n; i++)
	{
		Int16 tmp3 = input[i];
		Int16 tmp = second_order_IIR(tmp3, coeff, z_x, z_y);
		Int16 tmp2 = tmp3 - tmp;
		output[i] = ((tmp3 + tmp) >> 1)  +  (k > 0? tmp2 << (k - 1) : tmp2 >> (1 - k));
	}
}


float calculateAlpha(float omega)
{
	float a1 = 1/cos(omega) + tan(omega);
	float a2 = 1/cos(omega) - tan(omega);

	return (a1 >= -1 && a1 <= 1)? a1 : a2;
}


void setAlphaBeta(float OmegaLP, float OmegaP1, float BOmegaP1, float OmegaP2, float BOmegaP2, float OmegaHP)
{
	calculateShelvingCoeff(calculateAlpha(OmegaLP), coeff_lp);
	calculateShelvingCoeff(calculateAlpha(OmegaHP), coeff_hp);
	calculatePeekCoeff(calculateAlpha(BOmegaP1), cos(OmegaP1), coeff_m1);
	calculatePeekCoeff(calculateAlpha(BOmegaP2), cos(OmegaP2), coeff_m2);
}


void equalize(Int16* input, Int16 n, int *k, Int16* output)
{
	Int16 z_x2[2] = { 0 }, z_y2[2] = { 0 };
	Int16 z_x3[3] = { 0 }, z_y3[3] = { 0 };

	shelvingLP(input, coeff_lp, z_x2, z_y2, n, k[0] - 2, input);
	shelvingPeek(input, coeff_m1, z_x3, z_y3, n, k[1] - 2, input);

	memset(z_x2, 0, sizeof(z_x2));
	memset(z_y2, 0, sizeof(z_y2));
	memset(z_x3, 0, sizeof(z_x3));
	memset(z_y3, 0, sizeof(z_y3));

	shelvingPeek(input, coeff_m2, z_x3, z_y3, n, k[2] - 2, input);
	shelvingHP(input, coeff_hp, z_x2, z_y2, n, k[3] - 2, output);
}
