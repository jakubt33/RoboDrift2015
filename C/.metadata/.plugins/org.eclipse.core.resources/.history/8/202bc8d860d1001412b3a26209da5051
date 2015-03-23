/*
 * usart.h
 *
 *  Created on: 18 mar 2015
 *      Author: Kuba
 */

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#define BAUD 9600
#define FOSC 1000000
#define MYUBRR FOSC/16/BAUD-1

ISR(USART_RXC_vect);
ISR(SPI_STC_vect);
void USART_Transmit( unsigned char);
void spi_send(char);

#endif /* COMMUNICATION_H_ */
