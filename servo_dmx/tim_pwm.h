#ifndef __PWM__
#define __PWM__

#include "iostm8s003f3.h"
#include "inttypes.h"
#include "intrinsics.h"
#include "spi.h"

static uint16_t timer2_arr = 800; // PWM with 1000Hz frequency
static uint16_t timer2_ccr1 = 400; // 50% duty cycle.
extern uint8_t pwm_buffer[4];
extern uint8_t angle[2];

void tim2_pwm_init();
void pwm_enable(uint8_t mode);
void pwm_a();
void pwm_b();
void go_to_angle();
#endif