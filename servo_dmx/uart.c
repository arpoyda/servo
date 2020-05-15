#include "uart.h"

void UART_init(uint32_t baudrate, uint32_t freq){
  
  rxtail = 0;
  rxhead = 0;
  txtail = 0;
  txhead = 0;
  
  PD_DDR_DDR5 = 1;  //TX
  PD_DDR_DDR6 = 0;  //RX
  
  PD_CR1_C15 = 0;
  
  PD_CR1_C16 = 0;
  PD_CR2_C26 = 0;
  
  
  uint32_t brr = freq/baudrate;  
  UART1_BRR2 = brr & 0x000F;
  UART1_BRR2 |= brr >> 12;
  UART1_BRR1 = (brr >> 4) & 0x00FF;
  
  /*
  UART1_BRR1 = 0x08;
  UART1_BRR2 = 0x0A;
  */
  
  UART1_CR1_PIEN = 0;
  UART1_CR1_PCEN = 0;
  UART1_CR1_M = 0;
  UART1_CR1_UART0 = 0;
  
  UART1_CR2_TIEN = 0;
  UART1_CR2_TCIEN = 0;
  UART1_CR2_RIEN = 1;
  UART1_CR2_ILIEN = 0;

  
  UART1_CR2_TEN = 1;
  UART1_CR2_REN = 1;
  UART1_CR2_SBK = 0;
  
  UART1_CR3_STOP = 0;
}

uint16_t uart_tx(uint8_t *data, uint8_t len){
  uint8_t i;
  if (len < TXBUFSIZE){
    for (i=0; i<len; i++)
      txbuf[i] = data[i];
    
    txhead = len;
    txtail = 0;
    rxhead = 0;
    //UART1_CR2_TEN = 1;
    UART1_CR2_REN = 0;
    UART1_CR2_TCIEN = 1;

    return 1;
  }
  else{
    return 0;
  }
}

#pragma vector=UART1_R_RXNE_vector 
__interrupt void uart_rxne_interrupt(void){

  TIM1_CNTRH = 0;
  TIM1_CNTRL = 0;

  uint8_t head;
  head = rxhead;
  (void)UART1_SR;
  if ((head - rxtail) < RXBUFSIZE){
    rxbuf[head & (RXBUFSIZE - 1)] = UART1_DR;
    rxhead = head + 1;
  }
  else{
    (void)UART1_DR;
  }
}

#pragma vector=UART1_T_TXE_vector
__interrupt void uart_txe_interrupt(void){

  uint8_t tail;
  tail = txtail;
  while(!UART1_SR_TXE);
  UART1_DR = txbuf[tail & (TXBUFSIZE - 1)];
  tail++;
  txtail = tail;
  if (txhead == tail)
  {
    while(!UART1_SR_TC){};
    UART1_CR2_TCIEN = 0; /* disable TX interrupt */
    //UART1_CR2_TEN = 0;
    //PD_ODR_ODR5 = 0;
    UART1_CR2_REN = 1;
  }
}


uint8_t txbuf_ex[RXBUFSIZE];
uint8_t rxbuf_ex[TXBUFSIZE];
void tim1_uart_init(){
  __disable_interrupt();
  TIM1_PSCRH = timer1_pscr >> 8;
  TIM1_PSCRL = timer1_pscr & 0x00FF;

  TIM1_ARRH = timer1_arr >> 8;
  TIM1_ARRL = timer1_arr & 0x00FF;

  TIM1_CNTRH = 0;
  TIM1_CNTRL = 0;

  TIM1_IER_UIE = 1;
  TIM1_SR1_UIF = 0;

  TIM1_CR1_ARPE = 1;
  TIM1_CR1_CEN = 1;
  __enable_interrupt();
}

#pragma vector=TIM1_OVR_UIF_vector
__interrupt void tim1_uart_handler(void){

  uint8_t i;

  TIM1_SR1_UIF = 0;
  if (rxhead > 0){
    for(i=0; i<RXBUFSIZE; i++){
      rxbuf_ex[i] = rxbuf[i];
      rxbuf[i] = 0;
    }
    USART1_RXHandeler(rxbuf_ex, rxhead);
    rxhead = 0;
  }
  angle_request();
}



//tx - od