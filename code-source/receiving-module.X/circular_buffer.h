/* 
 * File:   CircularBuffer.h
 * Author: KapitanBomba
 *
 * Created on 4 grudnia 2019, 12:43
 */

#ifndef CIRCULARBUFFER_H
#define	CIRCULARBUFFER_H

unsigned char FrameBuffer(unsigned char *data,unsigned char mode);
#define WRITE       1
#define READ        0
#define NoData		0xFF
#define BuffFull	0xFE

#endif	/* CIRCULARBUFFER_H */

