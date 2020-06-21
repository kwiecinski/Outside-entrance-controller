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
    Global_Init();
    Interrupt_Init();
    I2C_Init();

    DataStruct data_rcv;
    KeyPointerStruct keydef;
    MenuParamPonterStruct menudef;
    TimeStruct time;
    
    Button_Init(&keydef);
    Menu_Init(&menudef);
  
    while(1)
    {   
       Check_RCV_Data(&data_rcv,&time,&menudef);
       Menu_Handler(&menudef,&keydef,&time);
    }
}