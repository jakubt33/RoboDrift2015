/*
 * usart.c
 *
 *  Created on: 18 mar 2015
 *      Author: Kuba
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"

volatile uint8_t spiDataReceived;

ISR(USART_RXC_vect)
{
    unsigned char data=0;
    data=UDR;

    if(data == 1){
    	LED1Y_ON;
    	LED2Y_OFF;
    	LED_RED_OFF;
    }
    else if(data==2){
    	LED2Y_ON;
    	LED1Y_OFF;
    	LED_RED_OFF;
    }
    else {
    	LED_RED_ON;
    	LED1Y_OFF;
    	LED2Y_OFF;
    }
}

ISR(SPI_STC_vect)
{
    char dataReceived = SPDR; //Pobieramy wartoœæ wys³an¹ przez Master
    spiDataReceived = dataReceived;
    //do sth
}

void spi_send(char data)
{
    SPDR = data;  //Wysy³amy zawartoœæ zmiennej bajt
    while( ! bit_is_set( SPSR, SPIF ) );        //Oczekujemy na zakoñczenie transmisji ( do ustawienia SPIF ) przez sprzêt
}

void USART_Transmit( unsigned char data)
{
    //wait for empty transmit buffer
    while ( !(UCSRA & (1<<UDRE)) );

    //put data into buffer, sends data
    UDR = data;
}
