/* 
 * 
 * 
 *
 * 
 */

#ifndef CIRCULARBUFFER_H
#define	CIRCULARBUFFER_H

unsigned char FrameBuffer(unsigned char *data,unsigned char mode);
#define WRITE       1
#define READ        0
#define NO_DATA		0xFF
#define BUFF_FULL	0xFE

#endif	/* CIRCULARBUFFER_H */

