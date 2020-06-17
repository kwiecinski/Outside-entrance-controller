/* 
 * 
 * 
 *
 * 
 */

#ifndef MANCHESTER_H
#define	MANCHESTER_H

void ManchesterDecode(unsigned char *edge_dir, unsigned int *pulse_time);
void ProcessRCVData(DataStruct *DataRCV, TimeStruct *time, MenuParamPonterStruct *time_limit);
unsigned char Check_Time_Date(TimeStruct *time, MenuParamPonterStruct *time_limit);


#endif	/* MANCHESTER_H */

