//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 2
// * Godina: 2016
// *
// * Zadatak: Ekvalizacija audio signala
// * Autor:
// *
// *
/////////////////////////////////////////////////////////////////////////////

#ifndef EQ_PROCESSING_H_
#define EQ_PROCESSING_H_
#include "tistdtypes.h"

void calculateShelvingCoeff(float alpha, Int16* output);

void calculatePeekCoeff(float alpha, float beta, Int16* output);

void shelvingHP(Int16* input, Int16* coeff, Int16* z_x, Int16* z_y, Int16 n, Int16 k, Int16* output);

void shelvingLP(Int16* input, Int16* coeff, Int16* z_x, Int16* z_y, Int16 n, Int16 k, Int16* output);

void shelvingPeek(Int16* input, Int16* coeff, Int16* z_x, Int16* z_y, Int16 n, Int16 k, Int16* output);

void setAlphaBeta(float OmegaL, float OmegaH, float OmegaP1, float BOmegaP1, float OmegaP2, float BOmegaP2);

void equalize(Int16* input, Int16 n, int kBass, int kTreble, int kMid1, int kMid2, Int16* output);

#endif /* EQ_PROCESSING_H_ */
