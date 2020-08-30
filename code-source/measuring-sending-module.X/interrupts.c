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
    //WDT(Timer0) Config PIC16f819 p.54
    OPTION_REGbits.T0CS=0;      //Internal instruction cycle clock (FOSC/4)
    OPTION_REGbits.PSA=1;       //Prescaler is assigned to the WDT module
    OPTION_REGbits.PS=0b111;    //Prescaler Rate Select bits: 1:64 (WDT) (reset ~2s)
      
    //Timer1 Config
    T1CONbits.TMR1CS=0;   //Timer1 uses internal oscillator Fosc/4
    T1CONbits.T1CKPS=0;   //Prescaler ratio: 1:1
    
    //Timer2 Config
    T2CONbits.TMR2ON=1;
    T2CONbits.TOUTPS=0b0001;    //Postscale 1:2
    T2CONbits.T2CKPS=0b01;      //Prescale 1:4
    PIE1bits.TMR2IE=1;          
           
    
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

    //Timer2 Interrupt
    if (TMR2IE && TMR2IF)
    {
        if(g_key_timer>0)
        {
           g_key_timer--; 
        }
         TMR2IF=0;
    }
}