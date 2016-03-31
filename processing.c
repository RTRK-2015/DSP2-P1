#include "processing.h"
#include "iir.h"
#include <math.h>
#include <stdlib.h>

#define FLT 32767

float aLP, aHP, aP1, aP2, bP1, bP2;


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
		Int16 tmp2 = input[i] + tmp;
		output[i] = ((input[i] - tmp) >> 1) + (k > 0? tmp2 << (k - 1) : tmp2 >> (k + 1));
	}
}

void shelvingLP(Int16* input, Int16* coeff, Int16* z_x, Int16* z_y, Int16 n, Int16 k, Int16* output)
{
	int i;

	for (i = 0; i < n; i++)
	{
		Int16 tmp = first_order_IIR(input[i], coeff, z_x, z_y);
		Int16 tmp2 = input[i] - tmp;
		output[i] = (k > 0? tmp2 << (k - 1) : tmp2 >> (k + 1)) + ((input[i] + tmp) >> 1);
	}
}

void shelvingPeek(Int16* input, Int16* coeff, Int16* z_x, Int16* z_y, Int16 n, Int16 k, Int16* output)
{
	int i;

	for (i = 0; i < n; i++)
	{
		Int16 tmp = second_order_IIR(input[i], coeff, z_x, z_y);
		Int16 tmp2 = input[i] - tmp;
		output[i] = ((input[i] + tmp) >> 1) + (k > 0? tmp2 << (k - 1) : tmp2 >> (k + 1));
	}
}

void setAlphaBeta(float OmegaLP, float OmegaHP, float OmegaP1, float BOmegaP1, float OmegaP2, float BOmegaP2)
{
	aLP = 1/cos(OmegaLP) + tan(OmegaLP);
	aHP = 1/cos(OmegaHP) + tan(OmegaHP);

	aP1 = 1/cos(BOmegaP1) + tan(BOmegaP1);
	bP1 = cos(OmegaP1);

	aP2 = 1/cos(BOmegaP2) + tan(BOmegaP2);
	bP2 = cos(OmegaP2);
}

void equalize(Int16* input, Int16 n, int kBass, int kTreble, int kMid1, int kMid2, Int16* output)
{
	Int16 coeff_lp[4];
	Int16 coeff_hp[4];
	Int16 coeff_m1[6];
	Int16 coeff_m2[6];

	Int16 z_x2[2], z_y2[2];
	Int16 z_x3[3], z_y3[3];

	Int16 tmp = (Int16*)malloc(n * sizeof(Int16));

	if (tmp == NULL)
	{
		printf("tmp1 NULL!");
		return;
	}


	calculateShelvingCoeff(aLP, coeff_lp);
	calculateShelvingCoeff(aHP, coeff_hp);
	calculatePeekCoeff(aP1, bP1, coeff_m1);
	calculatePeekCoeff(aP2, bP2, coeff_m2);

	shelvingLP(input, coeff_lp, z_x2, z_y2, n, kBass, tmp);
	shelvingPeek(tmp, coeff_m1, z_x3, z_y3, n, kMid1, output);

	memset(z_x2, 0, sizeof(z_x2));
	memset(z_y2, 0, sizeof(z_y2));
	memset(z_x3, 0, sizeof(z_x3));
	memset(z_y3, 0, sizeof(z_y3));

	shelvingPeek(output, coeff_m2, z_x3, z_y3, n, kMid2, tmp);
	shelvingHP(tmp, coeff_m2, z_x2, z_y2, n, kTreble, output);
}
