#include "inttypes.h"
#include "iostm8s003f3.h"
#include "uart.h"
#include "hal.h"
#include "intrinsics.h"

#define READ_INSTR 0x02
#define WRITE_INSTR 0x03
#define STATUS_INSTR 0x55
#define ERR_CODE 0

#define ID_DEFAULT 1

#define UART_BOUND 115200
#define F_MASTER 16000000


void USART1_RXHandeler(uint8_t* data, uint32_t rx_len);
void modbus_init(uint8_t mode);
uint16_t CRC16_2(uint8_t *buf, uint8_t len);

static uint8_t modbus_id;