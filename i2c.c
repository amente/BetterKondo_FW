/*
 * I2C Library for TM4C123GH6PM , uses I2C0
 */

#include "CU_TM4C123.h"
#include "i2c.h"

/*******************************************************************/
/*!
 * I2C Initialization
 * Set Baud Rate and turn on I2C0
 */
void I2C_init(void)
{

   SYSCTL->RCGCI2C |= 1UL<<0 ; // Enable clock gate for I2C0
   SYSCTL->RCGCGPIO |= 1UL<<1; // Enable clock gate for port B
   
   GPIOB ->AFSEL |= 1UL<<2 | 1UL<<3 ; // Enable alternate function for PB2 (I2C0SCL) and PB3(I2C0SDA)
   GPIOB -> ODR |= 1UL<<3  ; // PB3(I2C0SDA) set or open drain 
   GPIOB -> PCTL |= 3UL<<(4*2) | 3UL<<(4*3); // Set alternate function for PB2 - I2C0SCL and PB3 - I2C0SDA in PMC2 and PMC3   
   GPIOB-> DEN |= 1UL<<2 | 1UL<<3 ; // Enable PB2 and PB3
   
   
   I2C0 ->MCR = 1UL<<4;   // I2C0 Master function enable
   I2C0 ->MTPR = 7; // Assuming 16MHz clock, and SCL low pulse 6 and high pulse 4 set I2C datarate to 100Kbps

}

/*******************************************************************/
/*!
 * Start transmission and reads a register from a slave device
 * @param the slaveAddress
 * @param u8RegisterAddress is Register Address
 * @return Data stored in Register
 */
uint8_t I2C_ReadRegister(uint8_t u8SlaveAddress, uint8_t u8RegisterAddress)
{
	uint8_t result;
	
    // Assume I2C is in Idle state
	I2C0 ->MSA = (u8SlaveAddress<<1);  // Specify the slave address and set TX
    I2C0 ->MDR = u8RegisterAddress ; // Write register address to device
    I2C0 -> MCS = 3UL;  // Do start , transmit address and go to Master transmit state
    
    while(I2C0 -> MCS & 1UL<<0); // Wait while controller is busy
    
    I2C0 ->MSA = (u8SlaveAddress<<1)|1; // Specify the slave address and set RX       
    I2C0 -> MCS = 0xBUL;  // Do Repeat start , recieve data and go to Master recieve state
    
    while(I2C0 -> MCS & 1UL<<0); // Wait while controller is busy    
    result = I2C0 -> MDR;
    I2C0->MCS = 4UL; // Generate stop condition and go to Idle state	
	return result;
}

/**
 * Assume n>= 2
 */
void I2C_ReadMultiRegisters(uint8_t u8SlaveAddress, uint8_t u8RegisterAddress, uint8_t n, uint8_t *r)
{
	int8_t i;
    
    // Assume I2C is in Idle state
	I2C0 ->MSA = (u8SlaveAddress<<1);  // Specify the slave address and set TX
    I2C0 ->MDR = u8RegisterAddress ; // Write register address to device
    I2C0 -> MCS = 3UL;  // Do start , transmit address and go to Master transmit state
    
    while(I2C0 -> MCS & 1UL<<0); // Wait while controller is busy
    
    I2C0 ->MSA = (u8SlaveAddress<<1)|1; // Specify the slave address and set RX       
    I2C0 -> MCS = 0xBUL;  // Do Repeat start , recieve data and go to Master recieve state
    
    while(I2C0 -> MCS & 1UL<<0); // Wait while controller is busy 	
	
	
	// Recieve all other bytes except 2
	for(i=0; i<n-2; i++) 
	{
	    *r = I2C0 -> MDR; 
	    r++;
	    while(I2C0 -> MCS & 1UL<<0);
        I2C0 -> MCS = 0x9UL;
	}
	while(I2C0 -> MCS & 1UL<<0);
	*r = I2C0->MDR;
	r++;
    I2C0 -> MCS = 0x5UL;
    *r = I2C0->MDR;
	r++;
    while(I2C0 -> MCS & 1UL<<0);  
	
}

/*******************************************************************/
/*!
 *  Write a byte of data to a register on a slave device
 * @param the slaveAddress
 * @param u8RegisterAddress is Register Address
 * @param u8Data is Data to write
 */
void I2C_WriteRegister(uint8_t u8SlaveAddress, uint8_t u8RegisterAddress, int8_t u8Data)
{
    // Assume I2C is in Idle state
	I2C0 ->MSA = (u8SlaveAddress<<1);  // Specify the slave address and set TX
    
    I2C0 ->MDR = u8RegisterAddress ; // Write register address to device
  
    I2C0 -> MCS = 3UL;  // Do start , transmit address and go to Master transmit state
    
    while(I2C0 -> MCS & 1UL<<6); // Wait while the bus is busy
    
    I2C0 ->MDR = u8Data;
    
    I2C0 -> MCS  = 7UL;
     
    //TODO: Check error;    
          
}

