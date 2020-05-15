#include "spi.h"

uint8_t rxbuf_spi[2];
uint8_t rxbuf_spi_ex[2];


void SPI_init(){
        rxbuf_spi[0] = 0;
        rxbuf_spi[1] = 0;
        rxbuf_spi_ex[0] = 0;
        rxbuf_spi_ex[1] = 0;
	PC_DDR_DDR3 = 1;
  	PC_CR1_C13 = 1;
        PC_ODR_ODR3 = 1;
        
        PC_DDR_DDR5 = 1;
  	PC_CR1_C15 = 0;
        
	PC_DDR_DDR7 = 0;
  	PC_CR1_C17 = 0;
        
	//PC_DDR_DDR6 = 1;
  	//PC_CR1_C16 = 0;
        PC_DDR_DDR6 = 1;
  	PC_CR1_C16 = 1;
        PC_ODR_ODR6 = 1;

	SPI_CR1_LSBFIRST = 0;
	SPI_CR1_MSTR = 1;
	SPI_CR1_CPOL = 0;
	SPI_CR1_CPHA = 1;
	SPI_CR1_BR = 0x06; // F/16
        //SPI_CR2_SSI = 0;
        //SPI_CR2_SSM = 0;

	SPI_ICR_RXIE = 1;

	SPI_CR1_SPE = 1;
}

void angle_without_request(){
        uint8_t a = 0xE0;
        uint8_t b = 0x88;
	while(SPI_SR_BSY){};
        PC_ODR_ODR3 = 0; // CS -> LOW
	SPI_DR = 0x7F;
        while(b--){};
        SPI_DR =0xFE;

        while(a--){};
        PC_ODR_ODR3 = 1;        
}

void angle_request(){
        uint8_t a = 0xE0;
        uint8_t b = 0x88;
	while(SPI_SR_BSY){};
        PC_ODR_ODR3 = 0; // CS -> LOW
	SPI_DR = 0xFF;
        while(b--){};
        SPI_DR =0xFF;

        while(a--){};
        PC_ODR_ODR3 = 1;        
}

void field_request(){
        uint16_t a = 0xE0;
        uint8_t b = 0x88;
	while(SPI_SR_BSY){};
        PC_ODR_ODR3 = 0; // CS -> LOW
	SPI_DR = 0xFF;
        while(b--){};
        SPI_DR =0xFC;

        while(a--){};
        PC_ODR_ODR3 = 1;        
}


static uint16_t parity(uint16_t msg){
	uint16_t parity = 0;

	msg &= 0x7FFF;
	while(msg != 0)
	{
		parity ^= msg;
		msg >>= 1;
	}

	return (parity == (msg>>15));
}



#pragma vector = SPI_TXE_vector // one Vector for both interrupts
__interrupt void spi_txe_rxne_interrupt(void){ // one Function for both interrupts
	static uint8_t i = 1;

	if (SPI_SR_RXNE){
		if (i){
                        rxbuf_spi[0] = SPI_DR;
		}else{
			rxbuf_spi[1] = SPI_DR;
			//if (parity((rxbuf_spi[0]<<8) | rxbuf_spi[1])){
                        rxbuf_spi_ex[0] = rxbuf_spi[0] & 0x3FFF;
                        rxbuf_spi_ex[1] = rxbuf_spi[1];
                        //}
		}
                i = !i;
	}
}