/*
 * LPS25HB.h
 *
 *  Created on: July 12, 2023
 *      Author: bloup
 */

#ifndef INC_LPS25HB_H_
#define INC_LPS25HB_H_

#include "stm32f7xx_hal.h" //Needed of I2C
#include <math.h>

/*  D E F I N E S  */

#define LPS25HB_I2C_ADDR	0x5D << 1  //(0x5D << 1) //SAO == HIGH -> 0x5D, SAO == LOW -> 0xDC (p.25) bit need to be shifted left by 1 to make room for the R/W bit

#define LPS25HB_DEVICE_ID	0xBD


/*  R E G I S T E R S  */

#define LPS25HB_REG_REF_P_XL		0x08
#define LPS25HB_REG_REF_P_L			0x09
#define LPS25HB_REG_REF_P_H			0x0A
#define LPS25HB_WHO_AM_I			0x0F
#define LPS25HB_REG_RES_CONF		0x10
#define LPS25HB_REG_CTRL_REG1		0x20
#define LPS25HB_REG_CTRL_REG2		0x21
#define LPS25HB_REG_CTRL_REG3		0x22
#define LPS25HB_REG_CTRL_REG4		0x23
#define LPS25HB_REG_INTERRUPT_CFG	0x24
#define LPS25HB_REG_INT_SOURCE		0x25
#define LPS25HB_REG_STATUS_REG		0x27
#define LPS25HB_REG_PRESS_OUT_XL	0x28
#define LPS25HB_REG_PRESS_OUT_L		0x29
#define LPS25HB_REG_PRESS_OUT_H		0x2A
#define LPS25HB_REG_TEMP_OUT_L		0x2B
#define LPS25HB_REG_TEMP_OUT_H		0x2C
#define LPS25HB_REG_FIFO_CTRL		0x2E
#define LPS25HB_REG_FIFO_STATUS		0x2F
#define LPS25HB_REG_THS_P_L			0x30
#define LPS25HB_REG_THS_P_H			0x31
#define LPS25HB_REG_RPDS_L			0x39
#define LPS25HB_REG_RPDS_H			0x3A


/*  S E N S O R   S T R U C T  */

typedef struct
{
	I2C_HandleTypeDef *i2cHandle; //name of the I2C bus

	float pressure_hPa;

	float temp_C;

} LPS25HB;


/*  I N I T I A L I S A T I O N  */

uint8_t LPS25HB_Initialise( LPS25HB *dev, I2C_HandleTypeDef *i2cHandle );


/*  D A T A   A C Q U I S I T I O N  */

HAL_StatusTypeDef LPS25HB_ReadTemperature( LPS25HB *dev );
HAL_StatusTypeDef LPS25HB_ReadPressure( LPS25HB *dev );
float LPS25HB_FindAltitude(LPS25HB *dev);

/*  L O W - L E V E L   F U N C T I O N S  */

HAL_StatusTypeDef LPS25HB_ReadRegister( LPS25HB *dev, uint8_t reg, uint8_t *data );
HAL_StatusTypeDef LPS25HB_ReadMultipleRegisters( LPS25HB *dev, uint8_t reg, uint8_t *data, uint8_t length );

HAL_StatusTypeDef LPS25HB_WriteRegister( LPS25HB *dev, uint8_t reg, uint8_t *data );
uint8_t testWhoAmI(I2C_HandleTypeDef *i2cHandle );
int16_t convUintToint(uint16_t value);

#endif /* INC_LPS25HB_H_ */
