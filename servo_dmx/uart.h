#ifndef __UART__
#define __UART__


#include "iostm8s003f3.h"
#include "inttypes.h"
#include "intrinsics.h"

#define UART_BOUND 115200
#define F_MASTER 16000000


#define RXBUFSIZE 64
#define TXBUFSIZE 64

#define DIV2BRR1(d) (((d) >> 4) & 0xFF)
#define DIV2BRR2(d) ((((d) >> 8) & 0xF0) | ((d) & 0xF))

static uint8_t volatile rxbuf[RXBUFSIZE];
static uint8_t txbuf[TXBUFSIZE];
static uint8_t volatile rxhead, txtail;
static uint8_t rxtail, txhead;

static uint16_t timer1_arr = 100; // 10000/100 = 100Hz
static uint16_t timer1_pscr = 1600-1; // 16000000/1600 = 10kHz
extern uint8_t txbuf_ex[RXBUFSIZE];
extern uint8_t rxbuf_ex[TXBUFSIZE];

void UART_init(uint32_t baudrate, uint32_t freq);
uint16_t uart_tx(uint8_t *data, uint8_t len);
void tim1_uart_init();

extern void USART1_RXHandeler(uint8_t *,uint32_t);
#endif