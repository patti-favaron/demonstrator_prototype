/*
 * chip_access.c
 *
 *  Created on: Oct 20, 2022
 *      Author: Patrizia Favaron
 *
 *  Copyrights 2022 by Moresense srl
 *                     All rights reserved
 */

#include "chip_access.h"

uint8_t config(I2C_HandleTypeDef *hi2c1, struct state* tState) {

	// Variables declaration
	float   rHrelMultiplierRef;
	float   rHrelOffsetRef;
	int16_t iCountRelH0Ref;
	float   rRelH0Ref;
	int16_t iCountRelH1Ref;
	float   rRelH1Ref;
	uint32_t iChipIdRef;
	float   rHrelMultiplierDat;
	float   rHrelOffsetDat;
	int16_t iCountRelH0Dat;
	float   rRelH0Dat;
	int16_t iCountRelH1Dat;
	float   rRelH1Dat;
	uint32_t iChipIdDat;

	// Assume success (will falsify on failure)
	uint8_t iRetCode = 0;

	// Start reference and get configuration (channel 0)
	uint8_t iErrCode = hts221_setChannel(hi2c1, (uint8_t)0);
	if(iErrCode != 0) {
		iRetCode = 1;
		tState->iLastErrorRef = 100 + iErrCode;
		return iRetCode;
	}
	iErrCode = hts221_check(hi2c1);
	if(iErrCode != 0) {
		iRetCode = 2;
		tState->iLastErrorRef = 100 + iErrCode;
		return iRetCode;
	}
	iErrCode = hts221_setPattiPreferredState(hi2c1);
	if(iErrCode != 0) {
		iRetCode = 3;
		tState->iLastErrorRef = 100 + iErrCode;
		return iRetCode;
	}
	iErrCode = hts221_getCalibrationData(
		hi2c1,
		&rHrelMultiplierRef,
		&rHrelOffsetRef,
		&iCountRelH0Ref,
		&rRelH0Ref,
		&iCountRelH1Ref,
		&rRelH1Ref,
		&iChipIdRef
	);
	if(iErrCode != 0) {
		iRetCode = 4;
		tState->iLastErrorRef = 100 + iErrCode;
		return iRetCode;
	}

	// Get data from reference
	int16_t iRelHRef;
	iErrCode = hts221_getRawData(hi2c1, &iRelHRef);
	if(iErrCode != 0) {
		iRetCode = 5;
		tState->iLastErrorRef = 100 + iErrCode;
		return iRetCode;
	}

	// Start data chip and get configuration (channel 1)
	iErrCode = hts221_setChannel(hi2c1, (uint8_t)1);
	if(iErrCode != 0) {
		iRetCode = 6;
		tState->iLastErrorDat = 100 + iErrCode;
		return iRetCode;
	}
	iErrCode = hts221_check(hi2c1);
	if(iErrCode != 0) {
		iRetCode = 7;
		tState->iLastErrorDat = 100 + iErrCode;
		return iRetCode;
	}
	iErrCode = hts221_setPattiPreferredState(hi2c1);
	if(iErrCode != 0) {
		iRetCode = 8;
		tState->iLastErrorDat = 100 + iErrCode;
		return iRetCode;
	}
	iErrCode = hts221_getCalibrationData(
		hi2c1,
		&rHrelMultiplierDat,
		&rHrelOffsetDat,
		&iCountRelH0Dat,
		&rRelH0Dat,
		&iCountRelH1Dat,
		&rRelH1Dat,
		&iChipIdDat
	);
	if(iErrCode != 0) {
		iRetCode = 9;
		tState->iLastErrorDat = 100 + iErrCode;
		return iRetCode;
	}

	// Get data from data
	int16_t iRelHDat;
	iErrCode = hts221_getRawData(hi2c1, &iRelHDat);
	if(iErrCode != 0) {
		iRetCode = 10;
		tState->iLastErrorRef = 100 + iErrCode;
		return iRetCode;
	}

	// Fill initial state
	// -1- Chip configuration
	tState->iIdRef          = iChipIdRef;
	tState->iIdDat          = iChipIdDat;
	tState->rMultiplierRef  = rHrelMultiplierRef;
	tState->rMultiplierDat  = rHrelMultiplierDat;
	tState->rOffsetRef      = rHrelOffsetRef;
	tState->rOffsetDat      = rHrelOffsetDat;
	// -1- Static configuration
	tState->rDeltaThreshold = 5.0f;
	tState->rBeta           = 0.98f;
	tState->iLastErrorRef   = 0;
	tState->iLastErrorDat   = 0;
	// -1- Data (initial values)
	tState->iRawRef         = iRelHRef;
	tState->iRawDat         = iRelHDat;
	tState->rRef            = rHrelMultiplierRef * iRelHRef + rHrelOffsetRef;
	tState->rDat            = rHrelMultiplierDat * iRelHDat + rHrelOffsetDat;
	tState->rFilteredRef    = tState->rRef;
	tState->rFilteredDat    = tState->rDat;
	tState->rDelta          = 0.0f;
	tState->iResult         = 0;
	tState->iLastErrorRef   = 0;
	tState->iLastErrorDat   = 0;
	tState->iTimeStamp      = 0;

	// Leave
	return iRetCode;

}


uint8_t gather(I2C_HandleTypeDef *hi2c1, struct state* tState) {

	// Variables declaration
	int16_t iRelHRef;
	int16_t iRelHDat;

	// Assume success (will falsify on failure)
	uint8_t iRetCode = 0;

	// Start reference and get configuration (channel 0)
	uint8_t iErrCode = hts221_setChannel(hi2c1, (uint8_t)0);
	if(iErrCode != 0) {
		iRetCode = 1;
		tState->iLastErrorRef = 100 + iErrCode;
		return iRetCode;
	}
	iErrCode = hts221_getRawData(hi2c1, &iRelHRef);
	if(iErrCode != 0) {
		iRetCode = 5;
		tState->iLastErrorRef = 100 + iErrCode;
		return iRetCode;
	}

	// Start data chip and get configuration (channel 1)
	iErrCode = hts221_setChannel(hi2c1, (uint8_t)1);
	if(iErrCode != 0) {
		iRetCode = 1;
		tState->iLastErrorDat = 200 + iErrCode;
		return iRetCode;
	}
	iErrCode = hts221_getRawData(hi2c1, &iRelHDat);
	if(iErrCode != 0) {
		iRetCode = 2;
		tState->iLastErrorRef = 200 + iErrCode;
		return iRetCode;
	}

	// Fill initial state
	// -1- Data (initial values)
	tState->iRawRef         = iRelHRef;
	tState->iRawDat         = iRelHDat;
	tState->rRef            = tState->rMultiplierRef * iRelHRef + tState->rOffsetRef;
	tState->rDat            = tState->rMultiplierDat * iRelHDat + tState->rOffsetDat;
	tState->iLastErrorRef   = 0;
	tState->iLastErrorDat   = 0;

	// Leave
	return iRetCode;

}

