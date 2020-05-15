#include "hal.h"

int16_t read_register(uint16_t addr){
  if (addr == 0){
      return rxbuf_spi_ex[0];
  } else 
  if (addr == 1){
      return rxbuf_spi_ex[1];
  } else
  if (addr > 1 && addr < 10){
      return 0;
  }
  return -1;
}


int16_t write_register(uint16_t addr, uint8_t data){
  if (addr > 1 && addr < 6){
    pwm_buffer[addr-2] = data;
  } else 
  if (addr >=6 && addr < 8){
    angle[addr-6] = data;
  }else
  if (addr == 0xCC){
    if (data && (pwm_buffer[0] + pwm_buffer[1] > 0)){
      TIM2_ARRH = pwm_buffer[0];
      TIM2_ARRL = pwm_buffer[1];
      TIM2_CCR1H = pwm_buffer[2];
      TIM2_CCR1L = pwm_buffer[3];
    } else {
      pwm_enable(0);
    }
  } else 
  if (addr == 0xDD){
    if (data)
      go_to_angle();
  }
  return 1;
}

int16_t is_reg(uint16_t addr){
  if (addr <= 7 || addr == 0xCC || addr == 0xDD){
    return 1;
  } else {
    return 0;
  }
}