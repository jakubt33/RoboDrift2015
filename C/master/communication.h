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


volatile uint8_t spiDataReceived;
volatile uint8_t raceFlag;
volatile uint8_t race;

void USART_Transmit(unsigned char);
char spi_send(char);
void ping(uint8_t);
void startCounter(uint8_t);
void stopRace();
void sensorPreview(uint8_t);

#endif /* COMMUNICATION_H_ */
