/*
 * usart.h
 *
 *  Created on: 18 mar 2015
 *      Author: Kuba
 */

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#define BAUD 2400
#define FOSC 8000000
#define MYUBRR FOSC/16/BAUD-1

#define COMMAND_PING 0b00110011
#define COMMAND_PING_OK 0b00111111
#define COMMAND_PING_NO_OK 0b00110000

#define BEACTIVE_FALSE 0
#define BEACTIVE_TRUE 4
#define VEHICLE_IN 5

volatile uint8_t spiDataReceived;
//volatile uint8_t raceStart;
volatile uint8_t race;

uint8_t waitForSensor(uint8_t sensorNumber);
void USART_Transmit(unsigned char);
char spi_send(char);
void ping(uint8_t);
void startCounter(uint8_t);
void stopRace();
void numberPreview(uint8_t );
void sensorPreview(uint8_t);

#endif /* COMMUNICATION_H_ */
