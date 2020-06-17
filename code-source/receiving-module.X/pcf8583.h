/* 
 * 
 * 
 *
 * 
 */

#ifndef PCF8583_H
#define	PCF8583_H

#include "main.h"


void PCF8583_Read_Time_Date(TimeStruct *time_struct_ptr);
void RTC_init(TimeStruct *set0val);
void Time_To_UART(TimeStruct *time_struct_ptr);
void PCF8583_Set_Date_Time(TimeStruct *time_struct_ptr);
#endif	/* PCF8583_H */

