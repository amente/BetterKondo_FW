/*
 * I2C.h
 *
 *  Created on: Aug 31, 2014
 *      Author: amente
 */

#ifndef I2C_H_
#define I2C_H_

#include "CU_TM4C123.h"
#include <stdint.h>


void I2C_init(void);
void I2C_WriteRegister(uint8_t u8SlaveAddress,uint8_t u8RegisterAddress, int8_t u8Data);
uint8_t I2C_ReadRegister(uint8_t u8SlaveAddress, uint8_t u8RegisterAddress);
void I2C_ReadMultiRegisters(uint8_t u8SlaveAddress, uint8_t u8RegisterAddress, uint8_t n, uint8_t *r);



#endif /* I2C_H_ */
