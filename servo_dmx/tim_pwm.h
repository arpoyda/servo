#ifndef __PWM__
#define __PWM__

#include "iostm8s003f3.h"
#include "inttypes.h"
#include "intrinsics.h"
#include "spi.h"

static uint16_t timer2_arr = 999; // PWM with 2kHz frequency
static uint16_t timer2_ccr1 = 249; // 25% duty cycle.

void tim2_pwm_init();

#endif