/* 
 * File:   hw_i2c.h
 * Author: mpc
 *
 * Created on 14 kwietnia 2020, 00:06
 */

#ifndef HW_I2C_H
#define	HW_I2C_H

void Init_I2C(void);
void I2C_Master_Wait(void);
void I2C_Master_Start(void);
void I2C_Master_Stop(void);
void I2C_Master_Write(unsigned data);
unsigned char I2C_Master_Read(unsigned char ack);

#endif	/* HW_I2C_H */

