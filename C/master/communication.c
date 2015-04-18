/*
 * usart.c
 *
 *  Created on: 18 mar 2015
 *      Author: Kuba
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "io.h"
#include "communication.h"
#include "RFM69.h"

void ping(uint8_t numberOfSensors){
	uint8_t counter=1;
	for(counter=1;counter<=numberOfSensors;counter++){
		_delay_ms(200);
		_delay_ms(200);
		_delay_ms(200);
		_delay_ms(200);
		_delay_ms(200);
		sensorPreview(counter);
		if( sendWithRetry(counter+MASTER, COMMAND_PING,1,3) ){
			numberPreview(counter);
			_delay_ms(100);
			numberPreview(0);
		}
		else{
			LED_FALSTART_ON;
			_delay_ms(100);
			LED_FALSTART_OFF;
		}
	}
	sensorPreview(0);
	USART_Transmit(0b11111111);
}

void stopRace(){
	raceFlag=0;//stop sensors
	LED_RED_OFF;
	LED_FALSTART_OFF;
	LED_GO_OFF;
}

void sensorPreview(uint8_t SensorNumber){
	switch(SensorNumber){
	case 0:
		LED1S_OFF;
		LED2S_OFF;
		LED3S_OFF;
		LED4S_OFF;
		LED5S_OFF;
		break;
	case 1:
		LED1S_ON;
		LED2S_OFF;
		LED3S_OFF;
		LED4S_OFF;
		LED5S_OFF;
		break;
	case 2:
		LED1S_OFF;
		LED2S_ON;
		LED3S_OFF;
		LED4S_OFF;
		LED5S_OFF;
		break;
	case 3:
		LED1S_OFF;
		LED2S_OFF;
		LED3S_ON;
		LED4S_OFF;
		LED5S_OFF;
		break;
	case 4:
		LED1S_OFF;
		LED2S_OFF;
		LED3S_OFF;
		LED4S_ON;
		LED5S_OFF;
		break;
	case 5:
		LED1S_OFF;
		LED2S_OFF;
		LED3S_OFF;
		LED4S_OFF;
		LED5S_ON;
		break;
	default:
		LED1S_ON;
		LED2S_ON;
		LED3S_ON;
		LED4S_ON;
		LED5S_ON;
		break;
	}

}
void numberPreview(uint8_t SensorNumber){
	switch(SensorNumber){
	case 0:
		LED1Y_OFF;
		LED2Y_OFF;
		LED3Y_OFF;
		LED4Y_OFF;
		LED5Y_OFF;
		break;
	case 1:
		LED1Y_ON;
		LED2Y_OFF;
		LED3Y_OFF;
		LED4Y_OFF;
		LED5Y_OFF;
		break;
	case 2:
		LED1Y_OFF;
		LED2Y_ON;
		LED3Y_OFF;
		LED4Y_OFF;
		LED5Y_OFF;
		break;
	case 3:
		LED1Y_OFF;
		LED2Y_OFF;
		LED3Y_ON;
		LED4Y_OFF;
		LED5Y_OFF;
		break;
	case 4:
		LED1Y_OFF;
		LED2Y_OFF;
		LED3Y_OFF;
		LED4Y_ON;
		LED5Y_OFF;
		break;
	case 5:
		LED1Y_OFF;
		LED2Y_OFF;
		LED3Y_OFF;
		LED4Y_OFF;
		LED5Y_ON;
		break;
	default:
		LED1Y_ON;
		LED2Y_ON;
		LED3Y_ON;
		LED4Y_ON;
		LED5Y_ON;
		break;
	}
}

void startCounter(uint8_t number){
	switch (number){
	case 1:
		LED1Y_ON;
		raceFlag=0;
		break;
	case 2:
		LED2Y_ON;
		raceFlag=0;
		break;
	case 3:
		LED3Y_ON;
		raceFlag=0;
		break;
	case 4:
		LED4Y_ON;
		raceFlag=0;
		break;
	case 5:
		LED5Y_ON;
		raceFlag=0;
		break;
	case 0:
		LED1Y_OFF;
		LED2Y_OFF;
		LED3Y_OFF;
		LED4Y_OFF;
		LED5Y_OFF;
		LED_GO_ON;
		LED_FALSTART_OFF;
		raceFlag=1;
		race++;
		if(race>3) race=1;
		break;
	case 15:
		LED1Y_OFF;
		LED2Y_OFF;
		LED3Y_OFF;
		LED4Y_OFF;
		LED5Y_OFF;
		LED_GO_OFF;
		LED_FALSTART_ON;
		raceFlag=0;
		break;
	}
}

char spi_send(char data)
{
    SPDR = data;  //Wysy�amy zawarto�� zmiennej bajt
    while(!(SPSR & (1<<SPIF)));   //Oczekujemy na zako�czenie transmisji ( do ustawienia SPIF ) przez sprz�t
    return SPDR;
}

void USART_Transmit(unsigned char data)
{
    //wait for empty transmit buffer
    while ( !(UCSRA & (1<<UDRE)) );

    //put data into buffer, sends data
    UDR = data;
}
