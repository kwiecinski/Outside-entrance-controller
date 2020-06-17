#include <xc.h>
#include "main.h"
#include "interrupts.h"
#include "utils.h"
#include "hw_uart.h"
#include "manchester_decode.h"
#include "display-7-segment.h"

void InterruptConfig(void)
{
    //Timer0 Config
    OPTION_REGbits.T0CS=0;      //Internal instruction cycle clock (FOSC/4)
    OPTION_REGbits.PSA=0;       //Prescaler is assigned to the Timer0 module
    OPTION_REGbits.PS=0b000;    //Prescaler Rate Select bits: 1:2
    INTCONbits.T0IE=1;
    
    //TIMER1 Config
    T1CONbits.TMR1ON=1;     //Timer1 Enable
    T1CONbits.TMR1CS=0;     //Clock source - internal
    T1CONbits.T1CKPS=0b00;  //Prescaller 1:1
     
    //Interrupts Global
    INTCONbits.GIE=1;       //Global ISR Enable
    INTCONbits.PEIE=1;      //Peripheral Interrupt Enable
    
    //CCP1 Config
    FALLING_EDGE;
    PIE2bits.CCP2IE=1;   //CCP2 Interrupt Enable bit  
    
}

unsigned char g_display_text[4];
unsigned char g_decimal_point;

volatile unsigned char g_reciver_ccp2_isr_fire_flag;
volatile unsigned int g_pwm_freq, g_button_timer, g_generic_timer;

void __interrupt() 
ISR(void)
{
    static unsigned char edge_dir, set_flag, display_timer;
    unsigned int pulse_time;
      
    //////////////////////////////////////////////////////////////////////////
    //
    // Timer0 Interrupt - OVF - 256us
    //
    //////////////////////////////////////////////////////////////////////////

    if (TMR0IE && TMR0IF)
    {
        if(display_timer>15 && g_display_controll==1)
        {
            Display7SegmentText(&g_display_text[0],g_decimal_point);
            display_timer=0;
            
        }else
        {
            display_timer++;
        }
       
        if(g_button_timer)
        {
            g_button_timer--;
        }
        
        if(g_generic_timer)
        {
            g_generic_timer--;
        }
           
        TMR0IF=0; 
    }
    
    //////////////////////////////////////////////////////////////////////////
    //
    // CCP2 Interrupt (Capture Mode) - Manchester Decode
    //
    //////////////////////////////////////////////////////////////////////////
    
    if(CCP2IF && CCP2IE)
    {
        g_reciver_ccp2_isr_fire_flag=1;
        
        if(edge_dir==FALL_FLAG)				
        {
            RISING_EDGE;        
            edge_dir=RISE_FLAG;
          
        }else
        {
            FALLING_EDGE;                  
            edge_dir=FALL_FLAG;
        }	
        
        pulse_time=CCPR2;

        ManchesterDecode(&edge_dir,&pulse_time);
        
        TMR1=0;         //Null Timer1 
        CCP2IF=0;       //Clear CCP1 Interrupt Flag
    }
    
    //////////////////////////////////////////////////////////////////////////
    //
    // CCP1 Interrupt (Compare Mode) - PWM Speaker Modulation
    //
    //////////////////////////////////////////////////////////////////////////
    
    if(CCP1IF && CCP1IE)
    {
        if(set_flag)
        {
            COMPARE_SET;
            set_flag=0;
        }else
        {
            COMPARE_CLEAR;
            set_flag=1;
        }
        
        CCPR1=g_pwm_freq;
        TMR1=0;
        CCP1IF=0;
    }
}