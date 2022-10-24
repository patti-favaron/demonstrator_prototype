/*
 * data_set.h
 *
 *  Created on: Oct 20, 2022
 *      Author: Patrizia Favaron
 *
 *  Copyrights 2022 by Moresense srl
 *                     All rights reserved
 */

#ifndef INC_DATA_SET_H_
#define INC_DATA_SET_H_

// Main interchange and operations structure
// Keys:
//		(I) = Filled by 'config' procedure, accessible in read to 'anyone' else
//		(O) = Computed by 'process' procedure, and _possibly_ shown by GUI
//		(*) = Computed by 'process' procedure, and _mandatorily_ shown by GUI
struct state {
	// Chip configuration
	uint32_t iIdRef;								  // (I) Serial number of reference chip
	uint32_t iIdDat;								  // (I) Serial number of data chip
	float    rMultiplierRef;            			  // (I) Multiplier of reference "rel.humidity", computed from low-level calibration data from the HTS221s
	float    rOffsetRef;                			  // (I) Offset of reference "rel.humidity", computed from low-level calibration data from the HTS221s
	float    rMultiplierDat;            			  // (I) Multiplier of data "rel.humidity", computed from low-level calibration data from the HTS221s
	float    rOffsetDat;							  // (I) Offset of data "rel.humidity", computed from low-level calibration data from the HTS221s
	// Static configuration
	float    rDeltaThreshold;						  // (I) Threshold above which humidity differences are deemed significant
	float	 rBeta;									  // (I) Time-domain AR(1) filter coefficient
	// Data
	int16_t  iRawRef;							      // (*)  Raw reference relative humidity reading (counts)
	int16_t  iRawDat;							      // (*)  Raw reference relative humidity reading (counts)
	float    rRef;									  // (O)  Converted unfiltered reference relative humidity (%)
	float    rDat;									  // (O)  Converted unfiltered data relative humidity (%)
	float    rFilteredRef;							  // (O)  Converted filtered reference relative humidity (%)
	float    rFilteredDat;							  // (O)  Converted filtered data relative humidity (%)
	float    rDelta;								  // (O)  Relative humidity deltas (filtered, %)
	uint8_t  iResult;								  // (*)  Final result: 0 = Difference below threshold, not significant; 1 = Difference from threshold up, significant; 2 = Either reference or data failed
	// Activation state "last error found"
	uint8_t  iLastErrorRef;							  // (*)  From reference: 0: No error; 1: Not found; 2: Wrong chip answer; ...
	uint8_t  iLastErrorDat;							  // (*)  From data: 0: No error; 1: Not found; 2: Wrong chip answer; ...
	// Time stamp
	uint32_t iTimeStamp;							  // (*)  Integer iteration count
};

#endif /* INC_DATA_SET_H_ */
