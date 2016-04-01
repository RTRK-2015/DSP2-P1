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

#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "ezdsp5535_sar.h"
#include "print_number.h"
#include "math.h"
#include "string.h"
#include "stdlib.h"

#include "iir.h"
#include "processing.h"

/* Frekvencija odabiranja */
#define SAMPLE_RATE 8000L

#define PI 3.14159265

/* Niz za smestanje ulaznih i izlaznih odbiraka */
#pragma DATA_ALIGN(sampleBufferL,4)
Int16 sampleBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(sampleBufferR,4)
Int16 sampleBufferR[AUDIO_IO_SIZE];
short tempBuff[128];

void main( void )
{   
	int i;
	int k[4] = { 0 };
	int select = 0;

	Uint16 current = NoKey, last = NoKey;


	Int16 d[AUDIO_IO_SIZE] = { 10000 };
	Int16 z_x2[2], z_y2[2];
	Int16 z_x3[3], z_y3[3];

	Int16 shelving_lpp1[AUDIO_IO_SIZE];
	Int16 shelving_lpm1[AUDIO_IO_SIZE];
	Int16 shelving_hpp1[AUDIO_IO_SIZE];
	Int16 shelving_hpm1[AUDIO_IO_SIZE];

	Int16 peek_p1[AUDIO_IO_SIZE];
	Int16 peek_m1[AUDIO_IO_SIZE];

	Int16 shelving_coeff_lp[4];
	Int16 shelving_coeff_hp[4];
	Int16 peek_coeff[6];

	calculateShelvingCoeff(0.3, shelving_coeff_lp);
	calculateShelvingCoeff(-0.3, shelving_coeff_hp);
	calculatePeekCoeff(0.7, 0, peek_coeff);


    /* Inicijalizaija razvojne ploce */
    EZDSP5535_init( );

    /* Inicijalizacija kontrolera za ocitavanje vrednosti pritisnutog dugmeta*/
    EZDSP5535_SAR_init();

    /* Inicijalizacija LCD kontrolera */
    initPrintNumber();

	printf("\n Ekvalizacija audio signala \n");
		
    /* Inicijalizacija veze sa AIC3204 kodekom (AD/DA) */
    aic3204_hardware_init();
	
    /* Inicijalizacija AIC3204 kodeka */
	aic3204_init();

    aic3204_dma_init();
    
    /* Postavljanje vrednosti frekvencije odabiranja i pojacanja na kodeku */
    set_sampling_frequency_and_gain(SAMPLE_RATE, 0);

    setAlphaBeta(0, 0, 0, 0, 0, 0);

    /*EZDSP5535_OSD9616_printLetter(0x00,0x08,0x08,0x00);
    EZDSP5535_OSD9616_printLetter(0xff,0x81,0x81,0xff);
    EZDSP5535_OSD9616_printLetter(0x00,0xbf,0x00,0x00);*/
    printUp("-0");
    printDown("ii");

    while(1)
    {
    	/*memset(z_x2, 0, sizeof(z_x2));
    	memset(z_y2, 0, sizeof(z_y2));
    	shelvingLP(d, shelving_coeff_hp, z_x2, z_y2, AUDIO_IO_SIZE, 1, shelving_lpp1);

    	memset(z_x2, 0, sizeof(z_x2));
    	memset(z_y2, 0, sizeof(z_y2));
    	shelvingLP(d, shelving_coeff_hp, z_x2, z_y2, AUDIO_IO_SIZE, -1, shelving_lpm1);

    	memset(z_x2, 0, sizeof(z_x2));
    	memset(z_y2, 0, sizeof(z_y2));
    	shelvingHP(d, shelving_coeff_hp, z_x2, z_y2, AUDIO_IO_SIZE, 1, shelving_hpp1);

    	memset(z_x2, 0, sizeof(z_x2));
    	memset(z_y2, 0, sizeof(z_y2));
    	shelvingHP(d, shelving_coeff_hp, z_x2, z_y2, AUDIO_IO_SIZE, -1, shelving_hpm1);

    	memset(z_x3, 0, sizeof(z_x3));
    	memset(z_y3, 0, sizeof(z_y3));
    	shelvingPeek(d, peek_coeff, z_x3, z_y3, AUDIO_IO_SIZE, 1, peek_p1);

    	memset(z_x3, 0, sizeof(z_x3));
    	memset(z_y3, 0, sizeof(z_y3));
    	shelvingPeek(d, peek_coeff, z_x3, z_y3, AUDIO_IO_SIZE, -1, peek_m1);*/

    	//aic3204_read_block(sampleBufferL, sampleBufferR);

    	current = EZDSP5535_SAR_getKey();

    	if (current != last)
    	{
    		if (current == SW1)
    			select = (select + 1) % 4;
    		else if (current == SW2)
    			k[select] = (k[select] + 1) % 5;

    		last = current;
    	}

    	equalize(d, AUDIO_IO_SIZE, k, shelving_lpp1);
    	//equalize(sampleBufferR, AUDIO_IO_SIZE, k, sampleBufferR);

		//aic3204_write_block(sampleBufferR, sampleBufferR);
	}


	/* Prekid veze sa AIC3204 kodekom */
    aic3204_disable();

    printf( "\n***Kraj programa***\n" );
	SW_BREAKPOINT;
}


