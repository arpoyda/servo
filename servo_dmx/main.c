#include "iostm8s003f3.h"
#include "inttypes.h"
#include "uart.h"
#include "protocol.h"
#include "hal.h"
#include "intrinsics.h"
#include "tim_pwm.h"
#include "spi.h"

#define enableInterrupts()  asm("RIM")  /* enable interrupts */

void delay(unsigned long count) {
    while (count--)
        asm("nop");
}

int main( void )
{
  CLK_CKDIVR = 0;
  //=============================================
  PA_DDR_DDR2 = 1;
  PA_CR1_C12 = 1;
  PA_ODR_ODR2 = 1;
  //===================================================
  modbus_init(1);
  SPI_init();
  tim2_pwm_init();
  __enable_interrupt();
  uint8_t data[5] = {0x48,0x45,0x4c,0x4c,0x4f};
  uint8_t data1[3] = {0x00,0x00,0x00};
  while(1){
    int a = 2000000;
    while(a--);
    //angle_request();
    //uart_tx(data, 5);
    //uint16_t b = 50000;
    //while(b--){};
    //uart_tx(data1, 3);
    
  }
}
