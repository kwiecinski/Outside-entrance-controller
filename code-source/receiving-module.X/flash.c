/* 
 * 
 * 
 *
 * 
 */

#include "xc.h"
#include "flash.h"

void Write_EEprom(signed char data, unsigned char adress)
{
    EEADR=adress;
    EEDAT=data;
    INTCONbits.GIE=0; 
    WREN=1;
    EECON2=0x55;
    EECON2=0xAA;
    WR=1;
    while(!EEIF){}; 
    WREN=0;
    EEIF=0;
    INTCONbits.GIE=1; 
}

signed char Read_EEprom(unsigned char adress)
{
    INTCONbits.GIE=0;
    EEADR=adress;
    EEPGD=0;
    RD=1;
    INTCONbits.GIE=1; 
    return EEDAT;
}

