/*
*	LSM6DSR Accelerometer I2C Driver
*
*	Par : Air ESIEA (Evan Thibault et Denez Flegeau-Kihal)
*	Le : 12/07/2023
*
*	Sources :
*		-DataSheet, "https://www.st.com/resource/en/datasheet/lsm6dsr.pdf"
*		-Video, "https://www.youtube.com/watch?v=_JQAve05o_0&t=1586s&ab_channel=Phil%E2%80%99sLab"
*/

#ifndef LSM6DSR_I2C_DRIVER_H
#define LSM6DSR_I2C_DRIVER_H

#include "stm32f7xx_hal.h" /*Necessaire au fonctionnement de l'I2C*/

/*0x6B (SDO connected to the VDD)/0x6A (SDO connected to the GND)  The left shift frees the last bit*/
#define LSM6DSR_I2C_ADDR	(0x6B << 1)


/*REGISTER MAPPING*/
#define LSM6DSR_REG_FUNC_CFG_ACCESS				0x01
#define LSM6DSR_REG_PIN_CTRL					0x02
#define LSM6DSR_REG_S4S_TPH_L					0x04
#define LSM6DSR_REG_FUNC_CFG_ACCESS				0x01
#define LSM6DSR_REG_PIN_CTRL					0x02
#define LSM6DSR_REG_S4S_TPH_L					0x04
//#define LSM6DSR_REG_RESERVED 					0x03
#define LSM6DSR_REG_S4S_TPH_H 					0x05
#define LSM6DSR_REG_S4S_RR 						0x06
#define LSM6DSR_REG_FIFO_CTRL1 					0x07
#define LSM6DSR_REG_FIFO_CTRL2 					0x08
#define LSM6DSR_REG_FIFO_CTRL3 					0x09
#define LSM6DSR_REG_FIFO_CTRL4 					0x0A
#define LSM6DSR_REG_COUNTER_BDR_REG1 			0x0B
#define LSM6DSR_REG_COUNTER_BDR_REG2 			0x0C
#define LSM6DSR_REG_INT1_CTRL 					0x0D
#define LSM6DSR_REG_INT2_CTRL 					0x0E
#define LSM6DSR_REG_WHO_AM_I 					0x0F
#define LSM6DSR_REG_CTRL1_XL 					0x10
#define LSM6DSR_REG_CTRL2_G 					0x11
#define LSM6DSR_REG_CTRL3_C 					0x12
#define LSM6DSR_REG_CTRL4_C 					0x13
#define LSM6DSR_REG_CTRL5_C 					0x14
#define LSM6DSR_REG_CTRL6_C 					0x15
#define LSM6DSR_REG_CTRL7_G 					0x16
#define LSM6DSR_REG_CTRL8_XL 					0x17
#define LSM6DSR_REG_CTRL9_XL 					0x18
#define LSM6DSR_REG_CTRL10_C 					0x19
#define LSM6DSR_REG_ALL_INT_SRC 				0x1A
#define LSM6DSR_REG_WAKE_UP_SRC 				0x1B
#define LSM6DSR_REG_TAP_SRC 					0x1C
#define LSM6DSR_REG_D6D_SRC 					0x1D
#define LSM6DSR_REG_STATUS_REG 					0x1E
//#define LSM6DSR_REG_RESERVED 					0x1F
#define LSM6DSR_REG_OUT_TEMP_L 					0x20
#define LSM6DSR_REG_OUT_TEMP_H 					0x21
#define LSM6DSR_REG_OUTX_L_G 					0x22
#define LSM6DSR_REG_OUTX_H_G 					0x23
#define LSM6DSR_REG_OUTY_L_G 					0x24
#define LSM6DSR_REG_OUTY_H_G 					0x25
#define LSM6DSR_REG_OUTZ_L_G 					0x26
#define LSM6DSR_REG_OUTZ_H_G 					0x27
#define LSM6DSR_REG_OUTX_L_A 					0x28
#define LSM6DSR_REG_OUTX_H_A 					0x29
#define LSM6DSR_REG_OUTY_L_A 					0x2A
#define LSM6DSR_REG_OUTY_H_A 					0x2B
#define LSM6DSR_REG_OUTZ_L_A 					0x2C
#define LSM6DSR_REG_OUTZ_H_A 					0x2D
//#define LSM6DSR_REG_RESERVED 					0x2E-0x34
#define LSM6DSR_REG_EMB_FUNC_STATUS_MAINPAGE 	0x35
#define LSM6DSR_REG_FSM_STATUS_A_MAINPAGE 		0x36
#define LSM6DSR_REG_FSM_STATUS_B_MAINPAGE 		0x37
//#define LSM6DSR_REG_RESERVED 					0x38
#define LSM6DSR_REG_STATUS_MASTER_MAINPAGE 		0x39
#define LSM6DSR_REG_FIFO_STATUS1 				0x3A
#define LSM6DSR_REG_FIFO_STATUS2 				0x3B
//#define LSM6DSR_REG_RESERVED 					0x3C-0x3F
#define LSM6DSR_REG_TIMESTAMP0 					0x40
#define LSM6DSR_REG_TIMESTAMP1 					0x41
#define LSM6DSR_REG_TIMESTAMP2 					0x42
#define LSM6DSR_REG_TIMESTAMP3 					0x43
//#define LSM6DSR_REG_RESERVED 					0x44-0x55
#define LSM6DSR_REG_TAP_CFG0 					0x56
#define LSM6DSR_REG_TAP_CFG1 					0x57
#define LSM6DSR_REG_TAP_CFG2 					0x58
#define LSM6DSR_REG_TAP_THS_6D 					0x59
#define LSM6DSR_REG_INT_DUR2 					0x5A
#define LSM6DSR_REG_WAKE_UP_THS 				0x5B
#define LSM6DSR_REG_WAKE_UP_DUR 				0x5C
#define LSM6DSR_REG_FREE_FALL 					0x5D
#define LSM6DSR_REG_MD1_CFG 					0x5E
#define LSM6DSR_REG_MD2_CFG 					0x5F
#define LSM6DSR_REG_S4S_ST_CMD_CODE 			0x60
#define LSM6DSR_REG_S4S_DT_REG 					0x61
#define LSM6DSR_REG_I3C_BUS_AVB 				0x62
#define LSM6DSR_REG_INTERNAL_FREQ_FINE 			0x63
//#define LSM6DSR_REG_RESERVED 					0x64-0x6E
#define LSM6DSR_REG_INT_OIS 					0x6F
#define LSM6DSR_REG_CTRL1_OIS 					0x70
#define LSM6DSR_REG_CTRL2_OIS 					0x71
#define LSM6DSR_REG_CTRL3_OIS 					0x72
#define LSM6DSR_REG_X_OFS_USR 					0x73
#define LSM6DSR_REG_Y_OFS_USR 					0x74
#define LSM6DSR_REG_Z_OFS_USR 					0x75
//#define LSM6DSR_REG_RESERVED 					0x76-0x77
#define LSM6DSR_REG_FIFO_DATA_OUT_TAG 			0x78
#define LSM6DSR_REG_FIFO_DATA_OUT_X_L 			0x79
#define LSM6DSR_REG_FIFO_DATA_OUT_X_H 			0x7A
#define LSM6DSR_REG_FIFO_DATA_OUT_Y_L 			0x7B
#define LSM6DSR_REG_FIFO_DATA_OUT_Y_H 			0x7C
#define LSM6DSR_REG_FIFO_DATA_OUT_Z_L 			0x7D
#define LSM6DSR_REG_FIFO_DATA_OUT_Z_H 			0x7E

#define WHOAMI 									0x6B

/*SENSOR STRUCT*/
typedef struct {

	I2C_HandleTypeDef *i2cHandle;

	/*Acceleration X, Y, Z in m/s^2*/
	float acc_mps2[3];

	/*dunno*/
	float gyro[3];

	/*Temperature in degrees*/
	float temp_C;

}LSM6DSR;

/*TYPE*/
typedef struct {
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;

	uint8_t e;
	uint8_t f;
	uint8_t g;
	uint8_t h;

}bit;


/*INITIALISATION*/
uint8_t LSM6DSR_Initialise( LSM6DSR *dev, I2C_HandleTypeDef *i2cHandle );


/*DATA ACQUISITION*/
HAL_StatusTypeDef LSM6DSR_Read_Acceleration( LSM6DSR *dev );
HAL_StatusTypeDef LSM6DSR_Read_Gyroscope( LSM6DSR *dev );
HAL_StatusTypeDef LSM6DSR_Read_Temperature( LSM6DSR *dev );


/*LOW LEVEL FUNCTIONS*/
HAL_StatusTypeDef LSM6DSR_Read_Register( LSM6DSR *dev, uint8_t reg, uint8_t *data );
HAL_StatusTypeDef LSM6DSR_Read_Registers( LSM6DSR *dev, uint8_t reg, uint8_t *data, uint8_t length );

HAL_StatusTypeDef LSM6DSR_Write_Register( LSM6DSR *dev, uint8_t reg, uint8_t *data );


#endif/*LSM6DSR_I2C_DRIVER_H*/
