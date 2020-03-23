#ifndef __SPI__
#define __SPI__

#include "inttypes.h"
#include "iostm8s003f3.h"
#include "intrinsics.h"

#define ANGLECOM 0x3FFF

static uint8_t rxbuf_spi[2];
extern uint8_t rxbuf_spi_ex[2];

void SPI_init();
static uint16_t parity(uint16_t msg);
static void AS5047D_write(uint16_t addr, uint16_t data);
static void AS5047D_read(uint16_t addr);
void angle_without_request();
void angle_request();
void field_request();


#endif