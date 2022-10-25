/*
 * data_mng.c
 *
 *  Created on: Oct 20, 2022
 *      Author: Patrizia Favaron
 *
 *  Copyrights 2022 by Moresense srl
 *                     All rights reserved
 */

#include "math.h"
#include "data_mng.h"

uint8_t process(struct state* tState) {

	// Assume success (will falsify on failure)
	uint8_t iRetCode = 0;

	// Compute new filtered data
	tState->rFilteredRef = tState->rBeta * tState->rFilteredRef + (1.0f - tState->rBeta) * tState->rRef;
	tState->rFilteredDat = tState->rBeta * tState->rFilteredDat + (1.0f - tState->rBeta) * tState->rDat;

	// Compute delta over filtered data, and compare it to threshold
	tState->rDelta  = fabs(tState->rFilteredRef - tState->rFilteredDat);
	tState->iResult = tState->rDelta <= tState->rDeltaThreshold ? 0 : 1;

	// Update iteration count, to inform users measurements are progressing
	tState->iTimeStamp++;

	// Leave
	return iRetCode;

}

