#include <xc.h>
#include "main.h"
#include "inits.h"
#include "utils.h"
#include "interrupts.h"
#include "hw_uart.h"
#include "circular_buffer.h"
#include "manchester_decode.h"
#include "crc16.h"
#include "melody.h"
#include "display-7-segment.h"
#include "hw_i2c.h"
#include "pcf8583.h"
#include "menu.h"
#include "key-handler.h"


void main(void) 
{  
    Global_Inits();
    InterruptConfig();
    Init_I2C();
    UART_Init();
    
    DataStruct DataRCV;
    //TimeStruct time;  
    //time.seconds=0;
    //RTC_init(&time);
    
    g_display_text[0]=1;
    g_display_text[1]='f';
    g_display_text[2]='c';
    g_display_text[3]=4;
    g_decimal_point=0b0101;
    
    KeyPointerStruct keydef;
    Button_Init(&keydef);
    
    TimeStruct time;
    
    while(1)
    {   
        Button_Handler(&keydef);
        
        
        /*
        if(g_generic_timer==0)
        {
         PCF8583_Read_Time_Date(&time);
         SendUART(time.minutes);
            SendUART(5);
            g_generic_timer=4000;
        }
         */
       //  INTCONbits.GIE=0; 
        // PCF8583_Read_Time_Date(&time);
        //__delay_ms(400);
        //INTCONbits.GIE=1; 

        // Time_To_UART(&time);
        
       // CheckEvent(&DataRCV);
        
        ProcessRCVData(&DataRCV);
    }
}