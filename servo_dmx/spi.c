#include "spi.h"

uint16_t rxshort_ex, txshort_ex;

void SPI_init(){
	rxshort = 0;
	rxshort_ex = 0;

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




static uint16_t parity(uint16_t msg){
	uint16_t parity = 0;

	msg &= 0x7FFF;
	while(msg != 0)
	{
		parity ^= msg;
		msg >>= 1;
	}

	return (parity & 0x1);
}


void request_angle(){
        uint8_t a = 0xFF;
        uint8_t b = 0x88;
        PC_ODR_ODR3 = 0; // CS -> LOW
        //while(SPI_SR_BSY){};
        SPI_DR = 0xFF;
        //while(SPI_SR_BSY){};
        while(b--){};
	SPI_DR = 0xFF;
	//SPI_ICR_TXIE = 0;
        while(a--){};
        PC_ODR_ODR3 = 1; // CS -> HIGH    
}

#pragma vector = SPI_TXE_vector // one Vector for both interrupts
__interrupt void spi_txe_rxne_interrupt(void){ // one Function for both interrupts
	static uint8_t i = 1;

	if (SPI_SR_RXNE){
		if (i){
			rxshort = SPI_DR << 8;
		}else{
			rxshort |= SPI_DR;
			if (parity(rxshort)) rxshort_ex = rxshort;
		}
                i = !i;
	}
}