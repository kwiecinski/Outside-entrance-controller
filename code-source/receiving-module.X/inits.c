#include <xc.h>


void Global_Inits(void)
{
     
    //Ocillator Config - 16MHz Internal
    OSCCONbits.IRCF=0b111;
    OSCCONbits.OSTS=0;
    OSCCONbits.SCS=1;
    ANSEL=0x00;      //PORTA pins as digital I/O;
    ANSELH=0x00;
    
    
    
    TRISC0=0;    //unconeccted output
    TRISC1=1;    //radio rx
    TRISC2=0;    //speaker
    TRISC3=1;    //rtc sci
    TRISC4=1;    //rtc sda
    TRISC5=1;    //sw time2
    TRISC6=0;    //uart tx
    TRISC7=1;    //sw time1
    
    
    TRISA0=0;    //seg dp
    TRISA1=0;    //seg g
    TRISA2=0;    //seg f
    TRISA3=0;    //seg e
    TRISA4=0;    //seg d
    TRISA5=0;    //seg c
    TRISA6=0;    //seg a
    TRISA7=0;    //seg b
    
    TRISB0=1;    //sw set rtc
    TRISB1=1;    //sw right
    TRISB2=1;    //sw down
    TRISB3=1;    //sw up
    TRISB4=0;    //dig4 ca
    TRISB5=0;    //dig3 ca
    TRISB6=0;    //dig2 ca
    TRISB7=0;    //dig1 ca
    
   
}
