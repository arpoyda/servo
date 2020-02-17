#include "iostm8s003f3.h"
#include "stdint.h"

void UART_init(uint32_t baudrate, uint32_t freq);
void uart_tx(uint8_t *data, uint32_t len);
