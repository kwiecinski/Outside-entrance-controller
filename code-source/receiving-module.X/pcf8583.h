/* 
 * File:   pcf8583.h
 * Author: mpc
 *
 * Created on 14 kwietnia 2020, 00:30
 */

#ifndef PCF8583_H
#define	PCF8583_H

#include "main.h"


void PCF8583_Read_Time_Date(TimeStruct *time_struct_ptr);
void RTC_init(TimeStruct *set0val);
void Time_To_UART(TimeStruct *time_struct_ptr);
#endif	/* PCF8583_H */

