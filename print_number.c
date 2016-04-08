#include "ezdsp5535.h"
#include "ezdsp5535_lcd.h"
#include "ascii_digits.h"

void initPrintNumber()
{
	int i;

    EZDSP5535_OSD9616_init( );

    /* Zaustavi skrol teksta */
    EZDSP5535_OSD9616_send(0x00,0x2e);

    /* Ocisti ekran na stranici 0 */
    EZDSP5535_OSD9616_send(0x00,0x00);   // Postavi adresu
    EZDSP5535_OSD9616_send(0x00,0x10);
    EZDSP5535_OSD9616_send(0x00,0xb0+0); // Odaberi nultu stranicu

    /* Upisi nule */
    for(i=0; i<128; i++)
    {
        EZDSP5535_OSD9616_send(0x40,0x00);
    }

    /* Ocisti ekran na stranici 0 */
    EZDSP5535_OSD9616_send(0x00,0x00);   // Postavi adresu
    EZDSP5535_OSD9616_send(0x00,0x10);
    EZDSP5535_OSD9616_send(0x00,0xb0+1); // Odaberi prvu stranicu

    /* Upisi nule */
    for(i=0; i<128; i++)
    {
        EZDSP5535_OSD9616_send(0x40,0x00);
    }
}

void printNumber(Int16 x)
{
    int i;

    /* Postavi adresu na prvi karakter na nultoj stranici */
    EZDSP5535_OSD9616_send(0x00,0x00);
    EZDSP5535_OSD9616_send(0x00,0x10);
    EZDSP5535_OSD9616_send(0x00,0xb0+0);

    /* Ukoliko je zadati broj izmedju 0 - 9 ispisi ga */
    if(x >= 0 && x < 10)
    {
    	for(i = 0; i < sign_width; i++)
    	{
    		EZDSP5535_OSD9616_send(0x40, consolas_8ptBitmaps[(x+2)*sign_width+i]);
    	}
    	EZDSP5535_OSD9616_send(0x40, 0x00);
    }
    /* U suporotnom upisi neispravan karakter */
    else
    {
    	for(i = 0; i < sign_width; i++)
    	{
    	    EZDSP5535_OSD9616_send(0x40, 0xFF);
    	}
    	EZDSP5535_OSD9616_send(0x40, 0x00);

    }

}


void printChar(char x)
{
    int i;
	int index = 0;
	switch(x)
	{
		case '#':
			index = 0;
			break;
		case '*':
			index = 1;
			break;
		case '0':
			index = 2;
			break;
		case '1':
			index = 3;
			break;
		case '2':
			index = 4;
			break;
		case '3':
			index = 5;
			break;
		case '4':
			index = 6;
			break;
		case '5':
			index = 7;
			break;
		case '6':
			index = 8;
			break;
		case '7':
			index = 9;
			break;
		case '8':
			index = 10;
			break;
		case '9':
			index = 11;
			break;
		case 'A':
			index = 12;
			break;
		case 'B':
			index = 13;
			break;
		case 'C':
			index = 14;
			break;
		case 'D':
			index = 15;
			break;
		default:
			index = -1;
			break;
	}
    /* Postavi adresu na prvi karakter na nultoj stranici */
    EZDSP5535_OSD9616_send(0x00,0x00);
    EZDSP5535_OSD9616_send(0x00,0x10);
    EZDSP5535_OSD9616_send(0x00,0xb0+0);


	if(index >= 0)
	{
		for(i = 0; i < sign_width; i++)
		{
			EZDSP5535_OSD9616_send(0x40, consolas_8ptBitmaps[index*sign_width+i]);
		}
		EZDSP5535_OSD9616_send(0x40, 0x00);
    }
    /* U suporotnom upisi neispravan karakter */
    else
    {
    	for(i = 0; i < sign_width; i++)
    	{
    	    EZDSP5535_OSD9616_send(0x40, 0xFF);
    	}
    	EZDSP5535_OSD9616_send(0x40, 0x00);
    }
}


void truePrint(const char *str)
{
	int i;
	for (i = 0; i < 22 && *str; ++i, ++str)
	{
		switch (*str)
		{
		case 'T':
			EZDSP5535_OSD9616_printLetter(0x40,0x40,0x7f,0x40);
			break;

		case 'r':
			EZDSP5535_OSD9616_printLetter(0x1f, 0x10, 0x10, 0x10);
			break;

		case 'e':
			EZDSP5535_OSD9616_printLetter(0x1f, 0x15, 0x15, 0x1d);
			break;

		case 'b':
			EZDSP5535_OSD9616_printLetter(0x7f, 0x09, 0x09, 0x0f);
			break;

		case 'l':
			EZDSP5535_OSD9616_printLetter(0x00, 0x7f, 0x00, 0x00);
			break;

		case 'B':
			EZDSP5535_OSD9616_printLetter(0x7f, 0x49, 0x49, 0x7f);
			break;

		case 'a':
			EZDSP5535_OSD9616_printLetter(0x17, 0x15, 0x15, 0x1f);
			break;

		case 's':
			EZDSP5535_OSD9616_printLetter(0x1d, 0x15, 0x15, 0x17);
			break;

		case 'M':
			EZDSP5535_OSD9616_printLetter(0x7f, 0x20, 0x20, 0x7f);
			break;

		case 'i':
			EZDSP5535_OSD9616_printLetter(0x00,0x5f,0x00,0x00);
			break;

		case 'd':
			EZDSP5535_OSD9616_printLetter(0x0f, 0x09, 0x09, 0x7f);
			break;

		case '0':
			EZDSP5535_OSD9616_printLetter(0x7f,0x41,0x41,0x7f);
			break;

		case '1':
			EZDSP5535_OSD9616_printLetter(0x40, 0x7f, 0x00, 0x00);
			break;

		case '2':
			EZDSP5535_OSD9616_printLetter(0x4f, 0x49, 0x49, 0x79);
			break;

		case '-':
			EZDSP5535_OSD9616_printLetter(0x00,0x08,0x08,0x00);
			break;

		case ' ':
			EZDSP5535_OSD9616_printLetter(0x00,0x00,0x00,0x00);
			break;
		}
	}
}


void printUp(const char *str)
{
	int i;

    /* Write to page 0 */
    EZDSP5535_OSD9616_send(0x00,0x00);   // Set low column address
    EZDSP5535_OSD9616_send(0x00,0x10);   // Set high column address
    EZDSP5535_OSD9616_send(0x00,0xb1); // Set page for page 0 to page 5
    for(i=0;i<128;i++)
    	EZDSP5535_OSD9616_send(0x40,0x00);  // Spaces


	truePrint(str);
}


void printDown(const char *str)
{
	int i;

	EZDSP5535_OSD9616_send(0x00,0x00);   // Set low column address
	EZDSP5535_OSD9616_send(0x00,0x10);   // Set high column address
	EZDSP5535_OSD9616_send(0x00,0xb0); // Set page for page 0 to page 5

	for(i=0;i<128;i++)
		EZDSP5535_OSD9616_send(0x40,0x00);  // Spaces

	truePrint(str);
}
