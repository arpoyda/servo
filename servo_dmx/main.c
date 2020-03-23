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

void angle_send(){
        uint16_t a = 0x12C;
        uint8_t b = 0x88;
        uint8_t c = 0x30;
        while(c--);
	while(SPI_SR_BSY){};
        PC_ODR_ODR3 = 0; // CS -> LOW
	SPI_DR = 0xFF;
        while(b--){};
        SPI_DR =0xFF;

        while(a--){};
        PC_ODR_ODR3 = 1;        
}

void field_send(){
        uint16_t a = 0x12C;
        uint8_t b = 0x88;
	while(SPI_SR_BSY){};
        PC_ODR_ODR3 = 0; // CS -> LOW
        uint8_t c = 0x30;
        while(c--);
	SPI_DR = 0xFF;
        while(b--){};
        SPI_DR =0xFC;

        while(a--){};
        PC_ODR_ODR3 = 1;        
}




int main( void )
{
  CLK_CKDIVR = 0;
  //=============================================
  PA_DDR_DDR2 = 1;
  PA_CR1_C12 = 1;
  PA_ODR_ODR2 = 1;
  //===================================================
  //modbus_init(2);
  SPI_init();
  //tim2_pwm_init();
  //uint8_t data[4] = {0x01,0xAA,0x5,0x11};
  //enableInterrupts();
  __enable_interrupt();
  //uint8_t data[5] = {0x48,0x45,0x4c,0x4c,0x4f};
  //uart_tx(data, 5);
  while(1){
    
    int a = 10000;
    while(a--);
    //request_angle();
    field_send();
    //test_send(0xF2);
    
  }
}
