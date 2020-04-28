///////////////////////////////////////////////////////////////////////////////
//
//   
//
//
///////////////////////////////////////////////////////////////////////////////

#include <xc.h>
#include "manchester_encode.h"
#include "interrupts.h"
#include "sw_uart.h"

#define	GATE_INPUT             PORTAbits.RA4
#define	WICKET_INPUT           PORTAbits.RA3
#define WICKET_BELL_INPUT      PORTBbits.RB0
#define GATE_CLOSE_SWITCH      PORTAbits.RA1
#define GATE_CLOSE_OUTPUT      PORTBbits.RB3

void DebounceWait(void);
void GateStartHighTime(void);

//if line is high return 1
//if line is low return 0
unsigned char CheckInputState(unsigned char input)
{
	if(input==1)           
	{
        DebounceWait();
        
        if(input==1)            
        {
            return 1;          
        }
	}
    
    return 0;
}

void LineState(void)
{

    if(CheckInputState(GATE_INPUT)==1)
    {
         SendFrame(GATE);
        
    }else if(CheckInputState(WICKET_INPUT)==1)
    {
        SendFrame(WICKET);
        
    }else if(CheckInputState(WICKET_BELL_INPUT)==1)
    {
        SendFrame(WICKET_BELL);
    }
}

void CheckGateCloseSwitch(void)
{
	if(GATE_CLOSE_SWITCH==1)           
	{
        DebounceWait();

        if(GATE_CLOSE_SWITCH==1)            
        {
            GATE_CLOSE_OUTPUT=1;
            GateStartHighTime();
            GATE_CLOSE_OUTPUT=0;
        }
	}
}

void DebounceWait(void)
{
    //g_key_timer depends on timer interrupt time and desire debounce time
    //selected depending on hardware (empirically tested)
    g_key_timer=20;            //20ms    
    while(g_key_timer>0);   
}

//Used only in gate switch
void GateStartHighTime(void)
{
    //time depends on gate driver software
    //need to be long enough to driver see it
    //emirically tested
    g_key_timer=200;        //200ms    
    while(g_key_timer>0);   
}