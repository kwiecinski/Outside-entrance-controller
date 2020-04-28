#include <xc.h>


void GlobalInits(void)
{

    OSCCONbits.IRCF=0b111;      //Ocillator Config - 16MHz Internal
    ADCON1bits.PCFG=0b0110;     //Digital I/O AN0-AN4
    
    //I/O port directions
    TRISA0=0;       //unused pin
    TRISA1=1;       //gate sw
    TRISA2=0;       //radio TX
    TRISA3=1;       //wicket input
    TRISA4=1;       //gate input
    TRISA6=1;       //unused pin
    TRISA7=1;       //unused pin
    
    TRISB0=1;       //bell wicket input
    TRISB1=1;       //input - unused
    TRISB2=0;       //output - unused
    TRISB3=0;       //open gate output
    TRISB4=0;       //output - unused
    TRISB5=0;       //UART TX - for debuging
    
}
