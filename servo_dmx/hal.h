#ifndef __HAL__
#define __HAL__

#include "inttypes.h"
#include "iostm8s003f3.h"

int16_t read_register(uint16_t addr);
int16_t write_register(uint16_t addr, uint16_t data);
int16_t is_reg(uint16_t addr);

#endif