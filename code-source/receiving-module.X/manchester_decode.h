/* 
 * File:   Manchester.h
 * Author: KapitanBomba
 *
 * Created on 4 grudnia 2019, 11:56
 */

#ifndef MANCHESTER_H
#define	MANCHESTER_H

#define DecodeOK    1

void ManchesterDecode(unsigned char *EdgeDir, unsigned int *PulseTime);
void ProcessRCVData(DataStruct *DataRCV);
unsigned char CheckEvent(DataStruct *DataRCV);
#endif	/* MANCHESTER_H */

