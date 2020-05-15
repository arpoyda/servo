#ifndef __HAL__
#define __HAL__

#include "spi.h"
#include "inttypes.h"
#include "iostm8s003f3.h"
#include "tim_pwm.h"

int16_t read_register(uint16_t addr);
int16_t write_register(uint16_t addr, uint8_t data);
int16_t is_reg(uint16_t addr);

#endif