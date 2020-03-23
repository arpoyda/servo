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
  modbus_init(2);
  SPI_init();
  tim2_pwm_init();
  //uint8_t data[4] = {0x01,0xAA,0x5,0x11};
  //enableInterrupts();
  __enable_interrupt();
  //uint8_t data[5] = {0x48,0x45,0x4c,0x4c,0x4f};
  //uart_tx(data, 5);
  uint8_t zero_buf[2] = {0,0};
  while(1){
    
    int a = 200000;
    while(a--);
    //angle_request();
    uart_tx(zero_buf, 2);
    //angle_request();
    uart_tx(rxbuf_spi_ex, 2);
    
    //test_send(0xF2);`
    
  }
}
