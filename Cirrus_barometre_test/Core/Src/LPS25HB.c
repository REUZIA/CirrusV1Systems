/*
 * LPS25HB.c
 *
 *  Created on: Jul 12, 2023
 *      Author: bloup
 */

#include "LPS25HB.h"


/*  I N I T I A L I S A T I O N  */

uint8_t LPS25HB_Initialise( LPS25HB *dev, I2C_HandleTypeDef *i2cHandle )
{
	//Set Struct Parameters
	dev->i2cHandle		= i2cHandle;

	dev->pressure_hPa			= 0.0f;
	dev->temp_C					= 0.0f;

	//Crude and easy way to indicate an error occurred
	uint8_t errNum = 0;
	HAL_StatusTypeDef status;

	//Check device "identity"
	uint8_t regData=0;

	status = LPS25HB_ReadRegister( dev, LPS25HB_WHO_AM_I, &regData );
	errNum += ( status != HAL_OK );

	if( regData != LPS25HB_DEVICE_ID )
	{
		return 255;
	}


	regData = 0xC0;

	status = LPS25HB_WriteRegister( dev, LPS25HB_REG_CTRL_REG1, &regData ); //Datasheet page 35
	errNum += ( status != HAL_OK );

	return errNum;
}


/*  D A T A   A C Q U I S I T I O N  */

HAL_StatusTypeDef LPS25HB_ReadTemperature( LPS25HB *dev ) //Datasheet page 42 & 43
{
	uint8_t regData[2];

	HAL_StatusTypeDef status = 	LPS25HB_ReadMultipleRegisters( dev, LPS25HB_REG_TEMP_OUT_L | (1 << 7), regData, 2 );

	//Rearrange read data in regData correct order
	int16_t tempRaw = (int16_t)((regData[1] << 8) | regData[0]);

	//Voodoo crap to get the temp in degrees, page 9 of the Datasheet
	dev->temp_C = ((float) tempRaw / 480.0f) + 42.5f;

	 /*
	  HAL_I2C_Mem_Read(&hi2c1, LPS25HB_I2C_ADDR|0X01, LPS25HB_REG_TEMP_OUT_L , I2C_MEMADD_SIZE_8BIT, &(data[0]), 1, 500 );
	  HAL_I2C_Mem_Read(&hi2c1, LPS25HB_I2C_ADDR|0X01, LPS25HB_REG_TEMP_OUT_H , I2C_MEMADD_SIZE_8BIT, &(data[1]), 1, 500 );
	  */

	return status;
}

HAL_StatusTypeDef LPS25HB_ReadPressure( LPS25HB *dev ) //Datasheet page 41 & 42
{
	uint8_t regData[3];
	int32_t pressRaw=-1;
	HAL_StatusTypeDef status ;
	uint8_t i=0;
	//fixe problème capteur
	while (pressRaw<0 && i<250)
	{
		status= LPS25HB_ReadMultipleRegisters( dev, LPS25HB_REG_PRESS_OUT_XL| (1 << 7), regData, 3 );
		//Rearrange read data in the correct order
		pressRaw = ((int32_t)regData[2] << 16) | ((int16_t)regData[1] << 8) | (int8_t)regData[0]; // Combiner les données brutes en un entier signé de 24 bits
		i++;
	}



		//Voodoo crap to get the temp in degrees, page 9 of the Datasheet
		dev->pressure_hPa = ((float) pressRaw / 4096.0f);

		return status;
}

float LPS25HB_FindAltitude(LPS25HB *dev)
{
	//float altimeter_setting_mbar=1013.25;
	return (float)(-1)*(1 - powf((float)(dev->pressure_hPa / 1013.25), 0.190263)) * 44330.8;

}

/*  L O W - L E V E L   F U N C T I O N S  */

HAL_StatusTypeDef LPS25HB_ReadRegister( LPS25HB *dev, uint8_t reg, uint8_t *data )
{
	return HAL_I2C_Mem_Read(dev->i2cHandle, LPS25HB_I2C_ADDR|0X01, reg, I2C_MEMADD_SIZE_8BIT, data, 1, 500 );
}
//HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)

HAL_StatusTypeDef LPS25HB_ReadMultipleRegisters( LPS25HB *dev, uint8_t reg, uint8_t *data, uint8_t length )
{
	return HAL_I2C_Mem_Read(dev->i2cHandle, LPS25HB_I2C_ADDR|0X01, reg, I2C_MEMADD_SIZE_8BIT, data, length, 500 );
}

HAL_StatusTypeDef LPS25HB_WriteRegister( LPS25HB *dev, uint8_t reg, uint8_t *data )
{
	return HAL_I2C_Mem_Write(dev->i2cHandle, LPS25HB_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, 500 );
}

uint8_t testWhoAmI(I2C_HandleTypeDef *i2cHandle )
{
	  uint8_t whoAmIValue =10;
	  if (HAL_I2C_Mem_Read(i2cHandle, (0X5D<<1)|0x01, 0X0F, 1, &whoAmIValue, sizeof(whoAmIValue), HAL_MAX_DELAY) != HAL_OK ||  whoAmIValue!=0xBD ) {
		  return 1;

	  }
	  return 0;

}



int16_t convUintToint(uint16_t value)
{
	int16_t convertedValue;
	// Convertir uint16_t en int16_t
	if (value <= 32767) {
	    // La valeur est dans la plage positive de int16_t
	    return (int16_t)value;
	}
	// La valeur est dans la plage négative de int16_t
	return (int16_t)(value - 65536);
}

/*
uint8_t whoAmIRegValue = 0;

    // Envoyer l'adresse du registre WHO_AM_I
    HAL_StatusTypeDef result = HAL_I2C_Mem_Read(&hi2c1, LPS25HB_I2C_ADDR|0X01, WHO_AM_I_REG, I2C_MEMADD_SIZE_8BIT, &whoAmIRegValue, 1, HAL_MAX_DELAY);

    if (result == HAL_OK) {
        // Lire avec succès la valeur du registre WHO_AM_I
        return whoAmIRegValue;
    } else {
        // Erreur de lecture

    }
    */

