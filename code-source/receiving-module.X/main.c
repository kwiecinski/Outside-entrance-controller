/* 
 * 
 * 
 *
 * 
 */

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

    DataStruct data_rcv;
    KeyPointerStruct keydef;
    MenuParamPonterStruct menudef;
    TimeStruct time;
    
    Button_Init(&keydef);
    Menu_Init(&menudef);
    
   
    
    
    while(1)
    {   
        ProcessRCVData(&data_rcv,&time,&menudef);
        Select_Menu(&menudef,&keydef,&time);
    }
}