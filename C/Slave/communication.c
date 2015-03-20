/*
 * communication.c
 *
 *  Created on: 19 mar 2015
 *      Author: Kuba
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"

volatile uint8_t spiDataReceived;

ISR(SPI_STC_vect)
{
    char dataReceived = SPDR; //Pobieramy warto�� wys�an� przez Master
    spiDataReceived = dataReceived;
}

void spi_send(char data)
{
    SPDR = data;  //Wysy�amy zawarto�� zmiennej bajt
    while( ! bit_is_set( SPSR, SPIF ) );        //Oczekujemy na zako�czenie transmisji ( do ustawienia SPIF ) przez sprz�t
}