/* 
 * File:   newfile.h
 * Author: Dell
 *
 * Created on April 27, 2024, 11:43 AM
 */

#ifndef I2C_H
#define	I2C_H


//declaring the required functions for I2C Operations

void I2C_Master_Init(const unsigned long baud);          
void I2C_Master_Wait();
void I2C_Master_Start();
void I2C_Master_RepeatedStart();
void I2C_Master_Stop();
unsigned char I2C_Master_Write(unsigned char data);
unsigned char I2C_Read_Byte(void);
void I2C_ACK(void);
void I2C_NACK(void);
void EEPROM_Write(unsigned int add, unsigned char data);
unsigned char EEPROM_Read(unsigned int add);

#endif	

