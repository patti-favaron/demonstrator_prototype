/*
 * hts221_base.h
 *
 *  Created on: May 13, 2022
 *      Author: Patrizia Favaron
 *
 *  Copyrights 2022 by Moresense srl
 *                     All rights reserved
 */

#include "stm32wbxx_hal.h"
//#include "stm32l4xx_hal.h"

#ifndef HTS221_BASE_H_
#define HTS221_BASE_H_

// Multiplexer address
//
#define MUX_WRITE_ADDRESS 0xE0
#define MUX_READ_ADDRESS  0xE1

// HTS221 addresses and parameters
#define HTS221_REG_ID     0x0F
#define HTS221_ID         0xBC
#define HTS221_READ_ADR   0xBF
#define HTS221_WRITE_ADR  0xBE

// AVCONF register
#define HTS221_AVCONF_REGISTER          0x10
#define HTS221_AVCONF_TEMP_AVGS_2       0b00000000
#define HTS221_AVCONF_TEMP_AVGS_4       0b00001000
#define HTS221_AVCONF_TEMP_AVGS_8       0b00010000
#define HTS221_AVCONF_TEMP_AVGS_16      0b00011000
#define HTS221_AVCONF_TEMP_AVGS_32      0b00100000
#define HTS221_AVCONF_TEMP_AVGS_64      0b00101000
#define HTS221_AVCONF_TEMP_AVGS_128     0b00110000
#define HTS221_AVCONF_TEMP_AVGS_256     0b00111000
#define HTS221_AVCONF_RELH_AVGS_4       0b00000000
#define HTS221_AVCONF_RELH_AVGS_8       0b00001000
#define HTS221_AVCONF_RELH_AVGS_16      0b00010000
#define HTS221_AVCONF_RELH_AVGS_32      0b00011000
#define HTS221_AVCONF_RELH_AVGS_64      0b00100000
#define HTS221_AVCONF_RELH_AVGS_128     0b00101000
#define HTS221_AVCONF_RELH_AVGS_256     0b00110000
#define HTS221_AVCONF_RELH_AVGS_512     0b00111000
#define HTS221_AVCONF_PATTI_DEFAULT     0b00111111
#define HTS221_AVCONF_PATTI_2ND         0b00000000

// CTRL1 register
#define HTS221_CTRL1_REGISTER           0x20
#define HTS221_CTRL1_POWER_DOWN         0b10000000
#define HTS221_CTRL1_BLOCK_DATA_UPDATE  0b00000100
#define HTS221_CTRL1_DATA_RATE          0b00000011
#define HTS221_CTRL1_DATA_RATE_ONE_SHT  0b00000000
#define HTS221_CTRL1_DATA_1_HZ          0b00000001
#define HTS221_CTRL1_DATA_7_HZ          0b00000010
#define HTS221_CTRL1_DATA_12_5_HZ       0b00000011
#define HTS221_CTRL1_PATTI_DEFAULT      0b10000111

// CTRL2 register
#define HTS221_CTRL2_REGISTER           0x21
#define HTS221_CTRL2_BOOT               0b10000000
#define HTS221_CTRL2_HEATER             0b00000010
#define HTS221_CTRL2_HEATER_ON          0b00000010
#define HTS221_CTRL2_HEATER_OFF         0b11111101
#define HTS221_CTRL2_ONE_SHOT           0b00000001
#define HTS221_CTRL2_PATTI_DEFAULT      0b00000001

// Humidity calibration registers
#define HUMIDITY_RAW_0_LO               (0x36 | 0x80)
#define HUMIDITY_RAW_0_HI               (0x37 | 0x80)
#define HUMIDITY_RAW_1_LO               (0x3A | 0x80)
#define HUMIDITY_RAW_1_HI               (0x3B | 0x80)
#define HUMIDITY_PERCENT_0              (0x30 | 0x80)
#define HUMIDITY_PERCENT_1              (0x31 | 0x80)

// Temperature calibration registers (used to store binding-assigned chip ID)
#define TEMPERATURE_RAW_0_LO            (0x3C | 0x80)
#define TEMPERATURE_RAW_0_HI            (0x3D | 0x80)
#define TEMPERATURE_RAW_1_LO            (0x3E | 0x80)
#define TEMPERATURE_RAW_1_HI            (0x3F | 0x80)

// Relative humidity data
#define HUMIDITY_OUT_LO                 (0x28 | 0x80)
#define HUMIDITY_OUT_HI                 (0x29 | 0x80)

// Procedures
uint8_t hts221_setChannel(I2C_HandleTypeDef* hi2c1, const uint8_t iChannel);
uint8_t hts221_check(I2C_HandleTypeDef* hi2c1);
uint8_t hts221_setPattiPreferredState(I2C_HandleTypeDef* hi2c1);
uint8_t hts221_setPattiSecondPreferredState(I2C_HandleTypeDef* hi2c1);
uint8_t hts221_getCalibrationData(I2C_HandleTypeDef* hi2c1, float* rHrelMultiplier, float* rHrelOffset, int16_t* iCountRelH0, float* rRelH0, int16_t* iCountRelH1, float* rRelH1, uint32_t* iChipID);
uint8_t hts221_getRawData(I2C_HandleTypeDef* hi2c1, int16_t* iRelH);

#endif /* HTS221_BASE_H_ */
