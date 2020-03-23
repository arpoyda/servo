#include "tim_pwm.h"

void tim2_pwm_init() {
  __disable_interrupt();


  PD_DDR_DDR4 = 1; 
  PD_CR1_C14 = 1; 
  TIM2_CR1_ARPE = 1;
  TIM2_PSCR = 0x08; // 16MHz -> 2MHz

  TIM2_ARRH = timer2_arr >> 8;
  TIM2_ARRL = timer2_arr & 0x00FF;

  TIM2_CCR1H = timer2_ccr1 >> 8;
  TIM2_CCR1L = timer2_ccr1 & 0x00FF;

  TIM2_CNTRH = 0;
  TIM2_CNTRL = 0;

  TIM2_CCER1_CC1P = 1; // channel 1 active low
  TIM2_CCER1_CC1E = 1; // Enable channel 1 output

  TIM2_CCMR1 = (1 << 6) | (1 << 5) | (0 << 4); // Set output compare mode as 6 (0b110)
  
  TIM2_IER_CC1IE = 1;

  TIM2_CR1_CEN = 1; // Enable counter

  __enable_interrupt();
}

#pragma vector=TIM2_CAPCOM_CC1IF_vector
__interrupt void tim2_cc1_interrupt(void){
  if (TIM2_SR1_CC1IF){
    TIM2_SR1_CC1IF = 0;
    field_request();
  }
}