#include <xc.h>
#include <stdio.h>
#include "main.h"
#include "sw_uart.h"


///////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////

#define		BAUD_RATE_UART  	1200
#define     TX_HIGH            PORTBbits.RB5=1;
#define     TX_LOW             PORTBbits.RB5=0;

unsigned char DigitToString(unsigned char digit)
{
    unsigned char i;

    for(i=0;i<10;i++)
    {
        if(digit==i)
		{
			return('0'+i);
		}
    }      
    
    return 0;
}

void SendUART(unsigned char data) 
{
	TX_LOW;
	__delay_us(1000000/BAUD_RATE_UART);

	for(unsigned char i=0;i<8;i++)
	{
		if(data>>i & 0x01)
		{
			TX_HIGH;
		}else
		{
			TX_LOW;
		}
		__delay_us(1000000/BAUD_RATE_UART);
	}

	TX_HIGH;
	__delay_us(1000000/BAUD_RATE_UART);
}


void SendArrayUART(unsigned char *data, unsigned char size)
{
    unsigned char i;
    
    for(i=0;i<size;i++)
    {
        SendUART(*(data+i));
    }
    
}


void SendDigitUART(unsigned int data)
{
    if(data<=0xFF)
    {
        SendUART(DigitToString(data%1000/100));
        SendUART(DigitToString(data%100/10));
        SendUART(DigitToString(data%10));
       
        
    }else if((data>0x00FF) && (data<=0xFFFF))
    {
        SendUART(DigitToString(data%100000/10000)); 
        SendUART(DigitToString(data%10000/1000)); 
        SendUART(DigitToString(data%1000/100));
        SendUART(DigitToString(data%100/10));
        SendUART(DigitToString(data%10));
   
    }
        

}
*/
