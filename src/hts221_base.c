/*
 * hts221_base.c
 *
 *  Created on: Oct 20, 2022
 *      Author: Patrizia Favaron
 *
 *  Copyrights 2022 by Moresense srl
 *                     All rights reserved
 */

#include "hts221_base.h"
#include <math.h>

uint8_t hts221_setChannel(I2C_HandleTypeDef* hi2c1, const uint8_t iChannel) {

	// Assume success (will falsify on failure)
	uint8_t iRetCode = 0;

	// Check input parameters
	if(iChannel > 0x07) {
		iRetCode = 1;
		return iRetCode;
	}

	// Prepare the channel selection byte, according to MUX convention.
	// Notice that in this application only zero or one channel is open
	// at a given time.
	uint8_t iChannelSelect = (0x01 << iChannel);

	// Select channel. On the MUX in use, to do this it is necessary to write
	// the channel selection byte to the system address
	volatile HAL_StatusTypeDef iErrCode;
	iErrCode = HAL_I2C_Master_Transmit(hi2c1, MUX_WRITE_ADDRESS, &iChannelSelect, 1, 1000);
	if(iErrCode != HAL_OK) {
		iRetCode = 2;
	}

	// Wait a couple milliseconds to allow channel selection to settle
	HAL_Delay(2);

	// Leave
	return iRetCode;

}


uint8_t hts221_check(I2C_HandleTypeDef* hi2c1) {

	// Assume success (will falsify on failure)
	uint8_t iRetCode = 0;

	// Try to get the chip ID, and check it coincides with the one expected for HTS221
	volatile HAL_StatusTypeDef iErrCode;
	uint8_t ivBuffer[2];
	ivBuffer[0] = 0x00;
	iErrCode = HAL_I2C_Mem_Read(hi2c1, HTS221_READ_ADR, HTS221_REG_ID, I2C_MEMADD_SIZE_8BIT, ivBuffer, 1, 1000);
	if(iErrCode != HAL_OK) {
		iRetCode = 1;
		return iRetCode;
	}
	if(ivBuffer[0] != HTS221_ID) {
		iRetCode = 3;
	}
	return iRetCode;

}


uint8_t hts221_setPattiPreferredState(I2C_HandleTypeDef* hi2c1) {

	// Assume success (will falsify on failure)
	uint8_t iRetCode = 0;

	// Set AVCONF so that resolution is maximum for both humidity and temperature
	uint8_t ivBuffer[2];
	volatile HAL_StatusTypeDef iErrCode;
	ivBuffer[0] = HTS221_AVCONF_PATTI_DEFAULT;
	iErrCode = HAL_I2C_Mem_Write(hi2c1, HTS221_WRITE_ADR, HTS221_AVCONF_REGISTER, I2C_MEMADD_SIZE_8BIT, ivBuffer, 1, 1000);
	if(iErrCode != HAL_OK) {
		iRetCode = 1;
		return iRetCode;
	}

	// Set control register 1 so that data rate is maximum and HTS221 is active
	ivBuffer[0] = HTS221_CTRL1_PATTI_DEFAULT;
	iErrCode = HAL_I2C_Mem_Write(hi2c1, HTS221_WRITE_ADR, HTS221_CTRL1_REGISTER, I2C_MEMADD_SIZE_8BIT, ivBuffer, 1, 1000);
	if(iErrCode != HAL_OK) {
		iRetCode = 2;
		return iRetCode;
	}

	// Set control register 2 so that chip in active (boot done), and the heater is off
	ivBuffer[0] = HTS221_CTRL2_PATTI_DEFAULT;
	iErrCode = HAL_I2C_Mem_Write(hi2c1, HTS221_WRITE_ADR, HTS221_CTRL2_REGISTER, I2C_MEMADD_SIZE_8BIT, ivBuffer, 1, 1000);
	if(iErrCode != HAL_OK) {
		iRetCode = 3;
		return iRetCode;
	}

	// Leave
	return iRetCode;

}


uint8_t hts221_setPattiSecondPreferredState(I2C_HandleTypeDef* hi2c1) {

	// Assume success (will falsify on failure)
	uint8_t iRetCode = 0;

	// Set AVCONF so that resolution is maximum for both humidity and temperature
	uint8_t ivBuffer[2];
	volatile HAL_StatusTypeDef iErrCode;
	ivBuffer[0] = HTS221_AVCONF_PATTI_2ND;
	iErrCode = HAL_I2C_Mem_Write(hi2c1, HTS221_WRITE_ADR, HTS221_AVCONF_REGISTER, I2C_MEMADD_SIZE_8BIT, ivBuffer, 1, 1000);
	if(iErrCode != HAL_OK) {
		iRetCode = 1;
		return iRetCode;
	}

	// Set control register 1 so that data rate is maximum and HTS221 is active
	ivBuffer[0] = HTS221_CTRL1_PATTI_DEFAULT;
	iErrCode = HAL_I2C_Mem_Write(hi2c1, HTS221_WRITE_ADR, HTS221_CTRL1_REGISTER, I2C_MEMADD_SIZE_8BIT, ivBuffer, 1, 1000);
	if(iErrCode != HAL_OK) {
		iRetCode = 2;
		return iRetCode;
	}

	// Set control register 2 so that chip in active (boot done), and the heater is off
	ivBuffer[0] = HTS221_CTRL2_PATTI_DEFAULT;
	iErrCode = HAL_I2C_Mem_Write(hi2c1, HTS221_WRITE_ADR, HTS221_CTRL2_REGISTER, I2C_MEMADD_SIZE_8BIT, ivBuffer, 1, 1000);
	if(iErrCode != HAL_OK) {
		iRetCode = 3;
		return iRetCode;
	}

	// Leave
	return iRetCode;

}


uint8_t hts221_getCalibrationData(I2C_HandleTypeDef* hi2c1, float* rHrelMultiplier, float* rHrelOffset, int16_t* iCountRelH0, float* rRelH0, int16_t* iCountRelH1, float* rRelH1, uint32_t* iChipID) {

	// Assume success (will falsify on failure)
	uint8_t iRetCode = 0;

	// Get humidity point 0, in counts
	volatile HAL_StatusTypeDef iErrCode;
	uint8_t ivBuffer[2];
	ivBuffer[0] = 0x00;
	ivBuffer[1] = 0x00;
	iErrCode = HAL_I2C_Mem_Read(hi2c1, HTS221_READ_ADR, HUMIDITY_RAW_0_LO, I2C_MEMADD_SIZE_8BIT, ivBuffer, 2, 1000);
	if(iErrCode != HAL_OK) {
		iRetCode = 1;
		return iRetCode;
	}
	int16_t iRelHRaw0 = ivBuffer[0] | (ivBuffer[1] << 8);
	*iCountRelH0 = iRelHRaw0;

	// Get humidity point 1, in counts
	ivBuffer[0] = 0x00;
	ivBuffer[1] = 0x00;
	iErrCode = HAL_I2C_Mem_Read(hi2c1, HTS221_READ_ADR, HUMIDITY_RAW_1_LO, I2C_MEMADD_SIZE_8BIT, ivBuffer, 2, 1000);
	if(iErrCode != HAL_OK) {
		iRetCode = 2;
		return iRetCode;
	}
	int16_t iRelHRaw1 = ivBuffer[0] | (ivBuffer[1] << 8);
	*iCountRelH1 = iRelHRaw1;

	// Get humidity point 0, in percent
	ivBuffer[0] = 0x00;
	ivBuffer[1] = 0x00;
	iErrCode = HAL_I2C_Mem_Read(hi2c1, HTS221_READ_ADR, HUMIDITY_PERCENT_0, I2C_MEMADD_SIZE_8BIT, ivBuffer, 1, 1000);
	if(iErrCode != HAL_OK) {
		iRetCode = 3;
		return iRetCode;
	}
	float rRelHPhysical0 = ivBuffer[0] / 2.0f;
	*rRelH0 = rRelHPhysical0;

	// Get humidity point 1, in percent
	ivBuffer[0] = 0x00;
	ivBuffer[1] = 0x00;
	iErrCode = HAL_I2C_Mem_Read(hi2c1, HTS221_READ_ADR, HUMIDITY_PERCENT_1, I2C_MEMADD_SIZE_8BIT, ivBuffer, 1, 1000);
	if(iErrCode != HAL_OK) {
		iRetCode = 4;
		return iRetCode;
	}
	float rRelHPhysical1 = ivBuffer[0] / 2.0f;
	*rRelH1 = rRelHPhysical1;

	// Convert to relative humidity multiplier and offset
	*rHrelMultiplier = ((rRelHPhysical1 - rRelHPhysical0)) / (iRelHRaw1 - iRelHRaw0);
	*rHrelOffset     = (iRelHRaw1*rRelHPhysical0 - iRelHRaw0*rRelHPhysical1) / (iRelHRaw1 - iRelHRaw0);

	// Get temperature point 0, in counts (chip ID lower part)
	ivBuffer[0] = 0x00;
	ivBuffer[1] = 0x00;
	iErrCode = HAL_I2C_Mem_Read(hi2c1, HTS221_READ_ADR, TEMPERATURE_RAW_0_LO, I2C_MEMADD_SIZE_8BIT, ivBuffer, 2, 1000);
	if(iErrCode != HAL_OK) {
		iRetCode = 5;
		return iRetCode;
	}
	uint16_t iTempRaw0 = ivBuffer[0] | (ivBuffer[1] << 8);

	// Get temperature point 1, in counts (chip ID higher part)
	ivBuffer[0] = 0x00;
	ivBuffer[1] = 0x00;
	iErrCode = HAL_I2C_Mem_Read(hi2c1, HTS221_READ_ADR, TEMPERATURE_RAW_1_LO, I2C_MEMADD_SIZE_8BIT, ivBuffer, 2, 1000);
	if(iErrCode != HAL_OK) {
		iRetCode = 6;
		return iRetCode;
	}
	uint16_t iTempRaw1 = ivBuffer[0] | (ivBuffer[1] << 8);

	// Generate chip ID
	*iChipID = iTempRaw0 | (iTempRaw1 << 16);

	// Validate multiplier
	if(fabsf(*rHrelMultiplier) < 1.e-3f) {
		iRetCode = 7;
		return iRetCode;
	}

	// Leave
	return iRetCode;

}


uint8_t hts221_getRawData(I2C_HandleTypeDef* hi2c1, int16_t* iRelH) {

	// Assume success (will falsify on failure)
	uint8_t iRetCode = 0;

	// Get humidity, in counts
	volatile HAL_StatusTypeDef iErrCode;
	uint8_t ivBuffer[2];
	ivBuffer[0] = 0x00;
	ivBuffer[1] = 0x00;
	iErrCode = HAL_I2C_Mem_Read(hi2c1, HTS221_READ_ADR, HUMIDITY_OUT_LO, I2C_MEMADD_SIZE_8BIT, ivBuffer, 2, 1000);
	if(iErrCode == HAL_OK) {
		*iRelH = ivBuffer[0] | (ivBuffer[1] << 8);
	}
	else {
		*iRelH = -32768;
	}

	// Leave
	return iRetCode;
}

