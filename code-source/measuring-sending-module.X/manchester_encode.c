///////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include <xc.h>
#include "main.h"
#include "manchester_encode.h"
#include "sw_uart.h"
#include "crc16.h"
#include "interrupts.h"

#define	PREAMBLE    0xFE
#define MANCH_LOW        PORTAbits.RA2=0
#define MANCH_HIGH       PORTAbits.RA2=1

//baud rate can be set in InterruptConfig function

void WaitManchesterT(void)
{
    PIR1bits.CCP1IF=0;
    TMR1=0;
    while(PIR1bits.CCP1IF==0);
    
}

void ManchesterSendOne(void) 
{ 	 
	MANCH_LOW;
	WaitManchesterT();
	MANCH_HIGH;
	WaitManchesterT();
}

void ManchesterSendZero(void) 
{ 	
	MANCH_HIGH;
	WaitManchesterT();
	MANCH_LOW;
	WaitManchesterT();
}

void ManchesteEncode(unsigned char *data_to_send, unsigned char data_lenght)
{
	signed char bit_loop, data_loop;
	
	for(data_loop=0;data_loop<data_lenght;data_loop++)
	{
		for(bit_loop=7;bit_loop>=0;bit_loop--)
		{
			if(((*(data_to_send+data_loop))>>bit_loop) & 0x01)
			{
				ManchesterSendOne();
			}else
				ManchesterSendZero();
			}
	}

    MANCH_HIGH;
}

void SendFrame(unsigned char type)
{
    //adding to DATA_SIZE number of added preamble
    unsigned char data_table[DATA_SIZE+4]; 
   
    
	data_table[0]=PREAMBLE;
    data_table[1]=PREAMBLE;
    data_table[2]=PREAMBLE;
    data_table[3]=PREAMBLE;
                
	data_table[4]='$';
    
    if(type==GATE)
    {
        data_table[5]='G';
        data_table[6]='0';  
        
    }else if(type==WICKET)
    {
        data_table[5]='W';
        data_table[6]='0';  
        
    }else if(type==WICKET_BELL)
    {
        data_table[5]='W';
        data_table[6]='B';  
    }
    
    //counting CRC val strarting from 5 array element (data_table[5])
    //ending on 6 array element (second CRC16 function parameter is 2)
     unsigned int crc_val;
   	crc_val=CRC16(&data_table[5],2);   
   	data_table[7]=crc_val>>8;           //CRC first 8 bit				
	data_table[8]=crc_val & 0x00FF;     //CRC second 8bit
	data_table[9]='&';
    
    //disable timer0 to not affect timer1
    T1CONbits.TMR1ON=1;
    INTCONbits.TMR0IE=0;
    
    //sending the same data 3 times to minimize frame loss
    unsigned char i;
    for(i=0;i<3;i++)        
    {
        ManchesteEncode(&data_table[0], DATA_SIZE+4);
        //little time interval between send (grater than 2*ManchesterT)
        //to prevent frame overlap
        WaitManchesterT();
        WaitManchesterT();
        WaitManchesterT();
    }
    T1CONbits.TMR1ON=0;
    INTCONbits.TMR0IE=1;
}