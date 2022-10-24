/*
 * chip_access.h - Low-level chip interface (part of MUSE firmware in the real world;
 *                 included here to simulate MUSE firmware, and likely to be rewritten
 *                 completely by 221e)
 *
 *
 *  Created on: Oct 20, 2022
 *      Author: Patrizia Favaron
 *
 *  Copyrights 2022 by Moresense srl
 *                     All rights reserved
 */

#ifndef INC_CHIP_ACCESS_H_
#define INC_CHIP_ACCESS_H_

#include "stm32wbxx_hal.h"
#include "hts221_base.h"
#include "data_set.h"


uint8_t config(I2C_HandleTypeDef * hi2c1, struct state* tState);
uint8_t gather(I2C_HandleTypeDef * hi2c1, struct state* tState);

#endif /* INC_CHIP_ACCESS_H_ */
