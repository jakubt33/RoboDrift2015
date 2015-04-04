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

void ping(uint8_t numberOfSensors){
	/*
	uint8_t counter=1;
	for(counter=1;counter<=numberOfSensors;counter++){
		_delay_ms(500);
		if( ((counter==3)&&((race+1)==3)) || ((counter==4)&&((race+1)==3)) ){
			;
		}
		else{
			sensorPreview(counter);
			USART_Transmit(counter);
		}
	}
	_delay_ms(500);
	sensorPreview(0);
	USART_Transmit(0b11111111);*/
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
