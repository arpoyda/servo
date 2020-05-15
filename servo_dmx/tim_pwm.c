#include "tim_pwm.h"

uint8_t pwm_buffer[4] = {0, 0, 0, 0};
uint8_t angle[2] = {0x00, 0x00};

void tim2_pwm_init() {
  __disable_interrupt();
  PA_DDR_DDR1 = 1;
  PA_CR1_C11 = 1;
  PA_ODR_ODR1 = 1;
  

  PD_DDR_DDR4 = 1; 
  PD_CR1_C14 = 1; 
  //PA_DDR_DDR3 = 1; 
  //PA_CR1_C13 = 1; 
  
  TIM2_CR1_ARPE = 1;
  TIM2_PSCR = 0x0; // 16MHz -> 16MHz

  TIM2_ARRH = timer2_arr >> 8;
  TIM2_ARRL = timer2_arr & 0x00FF;

  TIM2_CCR1H = timer2_ccr1 >> 8;
  TIM2_CCR1L = timer2_ccr1 & 0x00FF;
  //TIM2_CCR3H = timer2_ccr1 >> 8;
  //TIM2_CCR3L = timer2_ccr1 & 0x00FF;

  TIM2_CNTRH = 0;
  TIM2_CNTRL = 0;

  TIM2_CCER1_CC1P = 1; // channel 1 active low
  TIM2_CCER1_CC1E = 1; // Enable channel 1 output
  //TIM2_CCER2_CC3P = 1; // channel 1 active low
  //TIM2_CCER2_CC3E = 1; // Enable channel 1 output

  TIM2_CCMR1 = (1 << 6) | (1 << 5) | (0 << 4); // Set output compare mode as 6 (0b110)
  //TIM2_CCMR3 = (1 << 6) | (1 << 5) | (0 << 4); // Set output compare mode as 6 (0b110)
  
  TIM2_IER_CC1IE = 0; // GADKAYA KAKASHKA

  TIM2_CR1_CEN = 1; // Enable counter

  __enable_interrupt();
}

void pwm_enable(uint8_t mode){
  if (mode)
    PA_ODR_ODR1 = 0;
  else
    PA_ODR_ODR1 = 1;
}

void pwm_a(){
  TIM2_CCER1_CC1E = 1; // Enable channel 1 output
  //TIM2_CCER1_CC2E = 0; // Enable channel 1 output
}
void pwm_b(){
  TIM2_CCER1_CC1E = 0; // Enable channel 1 output
  //TIM2_CCER1_CC2E = 1; // Enable channel 1 output
}

#pragma vector=TIM2_CAPCOM_CC1IF_vector
__interrupt void tim2_cc1_interrupt(void){
  if (TIM2_SR1_CC1IF){
    TIM2_SR1_CC1IF = 0;
    angle_request();
    uint16_t delta = (((rxbuf_spi_ex[0] & 0x3F) << 8) | rxbuf_spi_ex[1]) - (((angle[0] & 0x3F) << 8) | angle[1]);
    if (delta < 100){
      pwm_enable(0);
      TIM2_IER_CC1IE = 0;
    }
  }
}

void go_to_angle(){
  TIM2_IER_CC1IE = 1;
  pwm_enable(1);
}