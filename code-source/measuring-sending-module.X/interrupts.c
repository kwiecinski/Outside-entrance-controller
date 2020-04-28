///////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include <xc.h>
#include "interrupts.h"
#include "main.h"

void InterruptConfig(void)
{
    //Timer0 Config PIC16f819 p.54
    OPTION_REGbits.T0CS=0;      //Internal instruction cycle clock (FOSC/4)
    OPTION_REGbits.PSA=0;       //Prescaler is assigned to the Timer0 module
    OPTION_REGbits.PS=0b010;    //Prescaler Rate Select bits: 1:8
    INTCONbits.TMR0IE=1;        //Enable TMR0
    
    
    //Timer1 Config
    T1CONbits.TMR1CS=0;   //Timer1 uses internal oscillator Fosc/4
    T1CONbits.T1CKPS=0;   //Prescaler ratio: 1:1
    
    //Compare mode, generate software interrupt on match (CCP pin unaffected)
    CCP1CONbits.CCP1M=0b1010;
    
    //calculating CCPR1 value
    //formula assuming prescaler ratio 1:1
    const unsigned long int k_baud_rate=300;    //you can set your BR here
    CCPR1=_XTAL_FREQ/(4*k_baud_rate);        
    
    //Interrupts Global
    INTCONbits.GIE=1;       //Global ISR Enable
    INTCONbits.PEIE=1;      //Peripheral Interrupt Enable
}

void __interrupt() 
ISR(void)
{
    //////////////////////////////////////////////////////////////////////////
    //
    // Timer0 Interrupt -----  1,024ms
    // Formula: 1/(_XTAL_FREQ/4/prescaller/256) - Timer is 8bit
    //
    //////////////////////////////////////////////////////////////////////////

    //Timer0 Interrupt
    if (TMR0IE && TMR0IF)
    {
    
        if(g_key_timer>0)
        {
           g_key_timer--; 
        }
        TMR0IF=0;
    }
}