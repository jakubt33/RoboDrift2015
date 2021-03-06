/*
 * init.c
 *
 *  Created on: 17 mar 2015
 *      Author: Kuba
 */

#include "io.h"
#include <avr/io.h>

void init_SPI_master(){
	DDR_SPI |= (1 << SCK)|(1 << SS)|(1 << MOSI); // wyjcie na tych pinach
	SPCR = ( 1 << SPE ) | ( 1 << MSTR ) | ( 1 << SPR0 ); //spi enable, atmega = master, Prescaler 16
	//SPCR |= (1 << SPIE); //spi interrupt enable
}

void init_LED() {
	DDR_LED_Y |= (1 << LED1Y);
	DDR_LED_Y |= (1 << LED2Y);
	DDR_LED_Y |= (1 << LED3Y);
	DDR_LED_Y |= (1 << LED4Y);
	DDR_LED_Y |= (1 << LED5Y);

	DDR_LED_S124 |= (1 << LED1S);
	DDR_LED_S124 |= (1 << LED2S);
	DDR_LED_S35 |= (1 << LED3S);
	DDR_LED_S124 |= (1 << LED4S);
	DDR_LED_S35 |= (1 << LED5S);

	DDR_LED_FALSTART |= (1 << LED_FALSTART);
	DDR_LED_GO |= (1 << LED_GO);

	DDR_LED_RED |= (1 << LED_RED);

	LED_GO_OFF;
	LED_FALSTART_OFF;

	DDR_DIO &= ~(1<<DIO0);
}

void init_USART( unsigned char ubrr){
    // Ustawienie pr�dko�ci transmisji
    UBRRH = (uint8_t)(ubrr >> 8);
    UBRRL = (uint8_t)ubrr;

    // W��czenie odbiornika i nadajnika
    UCSRB |= (1 << RXEN) | (1 << TXEN);
    UCSRB |= (1 << RXCIE); //interrupt na receive

    // Ustawienie formatu ramki: 8 bit�w danych, 2 bit stopu
    UCSRC = (1 << URSEL) | (1 << USBS) | (3<<UCSZ0);
}
void init_RFM_RX_INT(){
	DDR_DIO &= ~(1<<DIO0);
	MCUCR |= (1<<ISC01)|(1<<ISC00); //rising egde generates interrupt request
	GICR |= (1<<INT0);
}
