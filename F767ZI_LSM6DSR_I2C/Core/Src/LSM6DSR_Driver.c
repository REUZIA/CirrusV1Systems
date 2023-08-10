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
#include "LSM6DSR_Driver.h"



uint8_t LSM6DSR_Initialise( LSM6DSR *dev, I2C_HandleTypeDef *i2cHandle ){

	/*Setup des para de la structure*/
	dev->i2cHandle		= i2cHandle;

	dev->acc_mps2[0]	= 0.0f;
	dev->acc_mps2[1]	= 0.0f;
	dev->acc_mps2[2]	= 0.0f;

	dev->gyro[0]		= 0.0f;
	dev->gyro[1]		= 0.0f;
	dev->gyro[2]		= 0.0f;

	dev->temp_C			= 0.0f;

	/*Stores number of transaction error*/
	uint8_t errNum = 0;
	HAL_StatusTypeDef status;


	/*Famous checking we talked about in the header*/
	/*Une unique vérification pour l'identité du capteur, celle du whoamI égal à l'ID du capteur*/
	uint8_t regData;

	status = LSM6DSR_Read_Register( dev, LSM6DSR_REG_WHO_AM_I, &regData );
	errNum += (status != HAL_OK );

	if(regData != WHOAMI ){

		return 255;
	}


		uint8_t data=0;
	    // Restore default configuration
	  //    lsm6dsr_reset_set(&dev_ctx, PROPERTY_ENABLE); + set

	    if (LSM6DSR_Read_Register(dev,LSM6DSR_REG_CTRL3_C,&data) == HAL_OK)
	    {
	      data|= (1 << 7);
	      status= LSM6DSR_Write_Register(dev,LSM6DSR_REG_CTRL3_C,&data);
	      errNum += (status != HAL_OK );
	    }

	    do {
	      LSM6DSR_Read_Register(dev,LSM6DSR_REG_CTRL3_C,(uint8_t*)&data);
	    } while ((data>>5)&1);


	    // Disable I3C interface
	    // ! lsm6dsr_i3c_disable_set(&dev_ctx, LSM6DSR_I3C_DISABLE);

	  if (LSM6DSR_Read_Register(dev, LSM6DSR_REG_CTRL9_XL,(uint8_t*)&data) == HAL_OK)
	  {
		  data |= (1 << 6);
		  LSM6DSR_Write_Register(dev, LSM6DSR_REG_CTRL9_XL,(uint8_t*)&data);
	  }



	  if (LSM6DSR_Read_Register(dev, LSM6DSR_REG_I3C_BUS_AVB, (uint8_t*)&data) == HAL_OK)
	  {
		  data|= (((uint8_t) 0x80U & 0x03U) << 3);
		  LSM6DSR_Write_Register(dev, LSM6DSR_REG_I3C_BUS_AVB,(uint8_t*)&data);
	  }




	    // Enable Block Data Update
	// lsm6dsr_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
/*
	    if (LSM6DSR_Read_Register(dev, LSM6DSR_REG_CTRL3_C, (uint8_t*)&data) == HAL_OK)
	    {
	    	data |=  (1 << 1);
			  LSM6DSR_Write_Register(dev, LSM6DSR_REG_CTRL3_C,(uint8_t*)&data);
	    }
*/


	  // acitver hight mode power/normal acellerometre / giro
	  if (LSM6DSR_Read_Register(dev, LSM6DSR_REG_CTRL6_C, (uint8_t*)&data) == HAL_OK)
	  {
		  data|= (((uint8_t) 0) << 4);
		  LSM6DSR_Write_Register(dev, LSM6DSR_REG_CTRL6_C,(uint8_t*)&data);
	  }
	  if (LSM6DSR_Read_Register(dev, LSM6DSR_REG_CTRL7_G, (uint8_t*)&data) == HAL_OK)
	  {
		  data|= (((uint8_t) 0) << 7);
		  LSM6DSR_Write_Register(dev, LSM6DSR_REG_CTRL7_G,(uint8_t*)&data);
	  }



	    // Set Output Data Rate

	    //lsm6dsr_xl_data_rate_set(&dev_ctx, LSM6DSR_XL_ODR_12Hz5);
	    //lsm6dsr_gy_data_rate_set(&dev_ctx, LSM6DSR_GY_ODR_12Hz5);

	    // Set full scale

		//Setting output data rate and digital filters for Acc

		regData = 	0x4E;
					//0100 1110 normal mode
					//1001 1110 hight


		status = LSM6DSR_Write_Register( dev, LSM6DSR_REG_CTRL1_XL, &regData);
		errNum += (status != HAL_OK );


		//Setting output data rate and digital filters for Gyro
		  uint8_t reg_data=0;
		  if (HAL_I2C_Mem_Read(dev->i2cHandle, LSM6DSR_I2C_ADDR, LSM6DSR_REG_CTRL2_G, I2C_MEMADD_SIZE_8BIT, &reg_data, 1, HAL_MAX_DELAY) == HAL_OK)
		  {
			  reg_data=0x48;//(((uint8_t)12) << 4);
			  if (HAL_I2C_Mem_Write(dev->i2cHandle, LSM6DSR_I2C_ADDR, LSM6DSR_REG_CTRL2_G, I2C_MEMADD_SIZE_8BIT, &reg_data, 1, HAL_MAX_DELAY) != HAL_OK) {
				  errNum+=1;
				}
		  }


/*
		regData = 	0x48;
					//0100 1000

		status = LSM6DSR_Write_Register( dev, LSM6DSR_REG_CTRL2_G, &regData);
		errNum += (status != HAL_OK );



		  if (LSM6DSR_Read_Register(dev, LSM6DSR_REG_CTRL2_G, &data) != HAL_OK)
		  {

			  data = 0x48;//(((uint8_t)12) << 4);
			  LSM6DSR_Write_Register(dev, LSM6DSR_REG_CTRL2_G,(uint8_t*)&data);
		  }
*/



	    // Configure filtering chain(No aux interface)
	    // Accelerometer - LPF1 + LPF2 path

		//    lsm6dsr_xl_hp_path_on_out_set(&dev_ctx, 0x04);

	  if (LSM6DSR_Read_Register(dev, LSM6DSR_REG_CTRL8_XL, (uint8_t*)&data) != HAL_OK)
	  {

		  data |= ((((uint8_t)0x04 & 0x10U) >> 4) << 5);
		  data |= ((((uint8_t)0x04 & 0x20U) >> 5) << 3);
		  data |= ((((uint8_t)0x04 & 0x07U) >> 5) << 0);
		  LSM6DSR_Write_Register(dev, LSM6DSR_REG_CTRL8_XL,(uint8_t*)&data);
	  }


	//    lsm6dsr_xl_filter_lp2_set(&dev_ctx, PROPERTY_ENABLE);
	  if (LSM6DSR_Read_Register(dev, LSM6DSR_REG_CTRL1_XL, (uint8_t*)&data) == HAL_OK)
	  {
		  data |= (1<<6);
		  LSM6DSR_Write_Register(dev, LSM6DSR_REG_CTRL1_XL,(uint8_t*)&data);
		  //output from LPF2 second filtering stage selected)
	  }

	  errNum += (status != HAL_OK );



	/*iF EQUAL TO 0 THEN SETUP IS SUCCESSFUL*/
	return errNum;
}


/*Read valus from sensor in 8 bits HIGH and 8 bits LOW*/
HAL_StatusTypeDef LSM6DSR_Read_Temperature( LSM6DSR *dev ){

	uint8_t regData[2];

	HAL_StatusTypeDef status = LSM6DSR_Read_Registers( dev, LSM6DSR_REG_OUT_TEMP_L, regData, 2 );

	int16_t tempRaw =(((int16_t)regData[1]) * 256) + (int16_t)regData[0];
			//( regData[1] << 16 | regData[0]<<8 );

	dev->temp_C =((float)tempRaw/ 256.0f) + 25.0f ;
	//((float) tempRaw / 480.0f) + 42.5f;

	return status;
}


/**/
HAL_StatusTypeDef LSM6DSR_Read_Acceleration( LSM6DSR *dev){

	uint8_t regData[6];

	HAL_StatusTypeDef status = LSM6DSR_Read_Registers( dev, LSM6DSR_REG_OUTX_L_A, regData, 6);

	int16_t accRaw[3];

	accRaw[0] = (int16_t)( ((int16_t) regData[1] * 256) + ((int16_t) regData[0] ) ); //Axe X
	accRaw[1] = (int16_t)( ((int16_t) regData[3] * 256) + ((int16_t) regData[2] ) ); //Axe Y
	accRaw[2] = (int16_t)( ((int16_t) regData[5] * 256) + ((int16_t) regData[4] ) ); //Axe Z
	/*
	accRaw[1] = (int16_t)( ((uint16_t) regData[3] << 8) | ((uint8_t) regData[2] ) ); //Axe Y
	accRaw[2] = (int16_t) ( ((uint16_t) regData[5] << 8) | ((uint8_t) regData[4] ) ); //Axe Z
	*/

	/*CONVERSION en m/s^2 */
	// 8g
	dev->acc_mps2[0] = 0.001f * 0.244f * 9.81 * (float)accRaw[0];
	dev->acc_mps2[1] = 0.001f * 0.244f * 9.81 * (float)accRaw[1];
	dev->acc_mps2[2] = 0.001f * 0.244f * 9.81 * (float)accRaw[2];

	return status;
}

HAL_StatusTypeDef LSM6DSR_Read_Gyroscope( LSM6DSR *dev){

	uint8_t regData[6];

	HAL_StatusTypeDef status = LSM6DSR_Read_Registers( dev, LSM6DSR_REG_OUTX_L_G, regData, 6);

	int16_t presRaw[3];

	presRaw[0] = (int16_t)( ((int16_t) regData[1] * 256) + ((int16_t) regData[0] ) ); //Axe X
	presRaw[1] = (int16_t)( ((int16_t) regData[3] * 256) + ((int16_t) regData[2] ) ); //Axe Y
	presRaw[2] = (int16_t)( ((int16_t) regData[5] * 256) + ((int16_t) regData[4] ) ); //Axe Z
	/*
	accRaw[1] = (int16_t)( ((uint16_t) regData[3] << 8) | ((uint8_t) regData[2] ) ); //Axe Y
	accRaw[2] = (int16_t) ( ((uint16_t) regData[5] << 8) | ((uint8_t) regData[4] ) ); //Axe Z
	*/

	/*CONVERSION en dps*/ // + compenser erreur
	dev->gyro[0] = 0.001f * 35.0f * (float)presRaw[0] ;
	dev->gyro[1] = 0.001f * 35.0f * (float)presRaw[1] ;
	dev->gyro[2] = 0.001f * 35.0f * (float)presRaw[2] ;

	return status;
}


HAL_StatusTypeDef LSM6DSR_Read_Register( LSM6DSR *dev, uint8_t reg, uint8_t *data ){

	return HAL_I2C_Mem_Read( dev->i2cHandle, LSM6DSR_I2C_ADDR , reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY );
}

HAL_StatusTypeDef LSM6DSR_Read_Registers( LSM6DSR *dev, uint8_t reg, uint8_t *data, uint8_t length ){

	return HAL_I2C_Mem_Read( dev->i2cHandle, LSM6DSR_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY );
}

HAL_StatusTypeDef LSM6DSR_Write_Register( LSM6DSR *dev, uint8_t reg, uint8_t *data ){

	return  HAL_I2C_Mem_Write( dev->i2cHandle, LSM6DSR_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY );
}
