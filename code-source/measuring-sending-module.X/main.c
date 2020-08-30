///////////////////////////////////////////////////////////////////////////////
//
//  Outside entrance controller
//  
//
///////////////////////////////////////////////////////////////////////////////

#include <xc.h>

#include "main.h"
#include "inits.h"
#include "interrupts.h"
#include "sw_uart.h"
#include "manchester_encode.h"
#include "input_handler.h"
#include "crc16.h"

void main(void) 
{
    GlobalInits();
    InterruptConfig();
  
    while(1)
    {
        CLRWDT();
        LineState();
        CheckGateCloseSwitch();
    }
    
}