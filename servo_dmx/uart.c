#include "uart.h"

void UART_init(uint32_t baudrate, uint32_t freq){
  
  uint32_t brr = freq/baudrate;
  
  PD_DDR_DDR5 = 1;  //TX
  PD_DDR_DDR6 = 0;  //RX
  
  PD_CR1_C16 = 0;
  PD_CR2_C26 = 0;
  
    
  UART1_BRR2 = brr & 0x000F;
  UART1_BRR2 |= brr >> 12;
  UART1_BRR1 = (brr >> 4) & 0x00FF;
  
  UART1_CR1_PIEN = 0;
  UART1_CR1_PCEN = 0;
  UART1_CR1_M = 0;
  UART1_CR1_UART0 = 0;
  
  UART1_CR2_TIEN = 0;
  UART1_CR2_TCIEN = 0;
  UART1_CR2_RIEN = 1;
  UART1_CR2_ILIEN = 0;
  __enable_interrupt();
  
  UART1_CR2_TEN = 1;
  UART1_CR2_REN = 1;
  UART1_CR2_SBK = 0;
  
  UART1_CR3_STOP = 0;
}

void uart_tx(uint8_t *data, uint32_t len){
  while(len--){
    while(!UART1_SR_TXE){};
    UART1_DR = *data++;
  }
}  

#pragma vector=UART1_R_RXNE_vector 
__interrupt void uart_rx_interrupt(void){
  uint8_t data;
  
  data = UART1_DR;
  
  //uart_tx_byte(data);
}

//tx - opendrein