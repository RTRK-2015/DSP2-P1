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
#define SAMPLE_RATE 16000L

#define PI 3.14159265

/* Niz za smestanje ulaznih i izlaznih odbiraka */
#pragma DATA_ALIGN(sampleBufferL,4)
Int16 sampleBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(sampleBufferR,4)
Int16 sampleBufferR[AUDIO_IO_SIZE];
short tempBuff[128];

void main( void )
{   
	int k[4] = { 0 };
	int select = 0;

	int ks[] = { 1, 3, 1, 2 };
	Int16 d[AUDIO_IO_SIZE] = { 10000 };
	Int16 o[AUDIO_IO_SIZE] = { 0 };


	Uint16 current = NoKey, last = NoKey;


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


    setAlphaBeta(/* Bass */ 220.0 / SAMPLE_RATE * 2 * PI,
				 /* Mid 1 */ 630.0 / SAMPLE_RATE * 2 * PI,
				 /* Mid 1*/ 330.0 / SAMPLE_RATE * 2 * PI,
				 /* Mid 2 */ 3300.0 / SAMPLE_RATE * 2 * PI,
				 /* Mid 2*/ 2200.0 / SAMPLE_RATE * 2 * PI,
				 /* Treble */ 6000.0 / SAMPLE_RATE * 2 * PI);

    printUp("Bass");
    printDown("-2");

    while(1)
    {
    	aic3204_read_block(sampleBufferL, sampleBufferR);

    	current = EZDSP5535_SAR_getKey();

    	if (current != last)
    	{
    		if (current == SW1)
    		{
    			select = (select + 1) % 4;
    			printUp(select == 0? "Bass" :
    			        select == 1? "Mid 1" :
    			        select == 2? "Mid 2" :
    			        			"Treble");
    		}
    		else if (current == SW2)
    		{
    			k[select] = (k[select] + 1) % 5;
    		}

    		last = current;


        	printDown(k[select] == 0? "-2" :
        			  k[select] == 1? "-1" :
        			  k[select] == 2? "0" :
        			  k[select] == 3? "1" :
        			  "2");
    	}


    	equalize(d, AUDIO_IO_SIZE, ks, o);
    	/*equalize(sampleBufferL, AUDIO_IO_SIZE, k, sampleBufferL);
    	equalize(sampleBufferR, AUDIO_IO_SIZE, k, sampleBufferR);*/

		aic3204_write_block(sampleBufferR, sampleBufferR);
	}


	/* Prekid veze sa AIC3204 kodekom */
    aic3204_disable();

    printf( "\n***Kraj programa***\n" );
	SW_BREAKPOINT;
}
