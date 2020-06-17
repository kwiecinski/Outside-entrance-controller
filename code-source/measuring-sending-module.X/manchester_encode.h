/* 
 * File:   Manchester.h
 * Author: KapitanBomba
 *
 * Created on 4 grudnia 2019, 11:56
 */

#ifndef MANCHESTER_ENCODE_H
#define	MANCHESTER_ENCODE_H

#define DATA_SIZE	 6

#define WICKET          0
#define GATE            1
#define WICKET_BELL     2

#define TRANSCIEVER_ON   PORTAbits.RA0=1
#define TRANSCIEVER_OFF  PORTAbits.RA0=0

void SendFrame(unsigned char type);

#endif

