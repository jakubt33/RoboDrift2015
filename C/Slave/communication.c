/*
 * communication.c
 *
 *  Created on: 19 mar 2015
 *      Author: Kuba
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "io.h"
#include "main.h"
#include "communication.h"
#include "RFM69.h"
#include "RFM69registers.h"


uint8_t checkPayload(){
	if ( receiveDone(0) ) { //0 because its not an answer to ACKrequest and there's no senderID yet
		if(dataReceived == '1'){
			showID(1);
		}
		else if(dataReceived=='2'){
			showID(2);
		}
		else showID(6);

		writeReg(REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN);
		return true;
	}
	else{
		return false;
	}
}
uint8_t spi_send(uint8_t data)
{
    SPDR = (unsigned char)data;  //Wysy³amy zawartoœæ zmiennej bajt
    while(!(SPSR & (1<<SPIF)));   //Oczekujemy na zakoñczenie transmisji ( do ustawienia SPIF ) przez sprzêt
    return (uint8_t)SPDR;
}

void checkButtons(){
	if( (RaceStart==0) && (bit_is_clear(PIN_SWITCH, SWITCH_OK)) ){ //only accessible when race has not started yet
		LED1_ON;
		LED2_ON;
		LED3_ON;
		LED4_ON;
		LED5_ON;
		_delay_ms(50);
		LED1_OFF;
		LED2_OFF;
		LED3_OFF;
		LED4_OFF;
		LED5_OFF;
		showID(SensorID);
		_delay_ms(50);

		uint8_t wait=1;
		while(wait){
			if(bit_is_clear(PIN_SWITCH, SWITCH_UP)){
				_delay_ms(50);
				SensorID++;
				if(SensorID>5){
					SensorID=1;
				}
				showID(SensorID);
			}
			if(bit_is_clear(PIN_SWITCH, SWITCH_DOWN)){
				_delay_ms(50);
				SensorID--;
				if(SensorID==255){
					SensorID=5;
				}
				showID(SensorID);
			}
			if(bit_is_clear(PIN_SWITCH, SWITCH_OK)){
				LED1_ON;
				LED2_ON;
				LED3_ON;
				LED4_ON;
				LED5_ON;
				_delay_ms(50);
				LED1_OFF;
				LED2_OFF;
				LED3_OFF;
				LED4_OFF;
				LED5_OFF;
				setAddress(SensorID+50);
				showID(SensorID);
				wait=0;
			}
		}
	}
}

void showID(uint8_t ID){
	switch (ID){
	case 1:
		LED1_ON;
		LED2_OFF;
		LED3_OFF;
		LED4_OFF;
		LED5_OFF;
		break;
	case 2:
		LED1_OFF;
		LED2_ON;
		LED3_OFF;
		LED4_OFF;
		LED5_OFF;
		break;
	case 3:
		LED1_OFF;
		LED2_OFF;
		LED3_ON;
		LED4_OFF;
		LED5_OFF;
		break;
	case 4:
		LED1_OFF;
		LED2_OFF;
		LED3_OFF;
		LED4_ON;
		LED5_OFF;
		break;
	case 5:
		LED1_OFF;
		LED2_OFF;
		LED3_OFF;
		LED4_OFF;
		LED5_ON;
		break;
	case 0:
		LED1_OFF;
		LED2_OFF;
		LED3_OFF;
		LED4_OFF;
		LED5_OFF;
		break;
	default:
		LED1_ON;
		LED2_ON;
		LED3_ON;
		LED4_ON;
		LED5_ON;
		break;
	}
}
