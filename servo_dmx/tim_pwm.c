#include "tim_pwm.h"

void tim2_pwm_init() {
  __disable_interrupt();

  // As per datasheet of stm8s103f3 PD4 pin is timer 2 channel 1.
  PD_DDR_DDR4 = 1; // 0b00010000 PD4 is now output
  PD_CR1_C14 = 1; // 0b00010000 PD4 is now pushpull
  TIM2_CR1_ARPE = 1;
  TIM2_PSCR = 0x08; // Prescaler = 8

  // Fill 16 bit timer2_arr to two 8 bit registers.
  // MSB register to be filled first.
  TIM2_ARRH = timer2_arr >> 8;
  TIM2_ARRL = timer2_arr & 0x00FF;

  // Fill 16 bit timer2_ccr1 to two 8 bit registers.
  // MSB register to be filled first.
  TIM2_CCR1H = timer2_ccr1 >> 8;
  TIM2_CCR1L = timer2_ccr1 & 0x00FF;

  TIM2_CCER1_CC1P = 1; // channel 1 active low
  TIM2_CCER1_CC1E = 1; // Enable channel 1 output

  // PWM mode 1.
  TIM2_CCMR1 = (1 << 6) | (1 << 5) | (0 << 4); // Set output compare mode as 6 (0b110)

  TIM2_CR1_CEN = 1; // Enable counter

  __enable_interrupt();
}
