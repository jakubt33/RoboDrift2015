/*
 * main.c
 *
 *  Created on: 17 mar 2015
 *      Author: Kuba
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "main.h"
#include "communication.h"
#include "io.h"
#include "init.h"
#include "RFM69registers.h"
#include "RFM69.h"

volatile unsigned char dataFromPC=0;
ISR(USART_RXC_vect){
    dataFromPC=UDR;
    char command = dataFromPC&0b11110000;
    char data = dataFromPC&0b00001111;
    if( command == 0b10000000){ //if 0b1xxx xxxx  //ping
    	dataFromPC=0;
    	ping(5/*0b00001111&dataFromPC*/); //get how many sensors to ping //0b00001111&dataFromPC
    }
    else if( command == 0b11000000){ //if 0bx1xx xxxx  //countdown
    	startCounter(data);
	    dataFromPC=0;
    }
    else if (command == 0b00100000){ //stop
    	dataFromPC=0;
    	stopRace();//disable sensors()
    }
    else if (dataFromPC != 0)  dataFromPC=0;
}

ISR(INT0_vect){ //payload ready
	if(SPI_receiveFlag == false){ //jeszeli nie otrzyma��m
		SPI_receiveFlag = true;
		if(receiveDone(0)){
			if( (dataReceived == VEHICLE_IN) ){
				sensorPreview(SENDERID-MASTER);
				USART_Transmit(SENDERID-MASTER); //poslij kompowi co otrzyma�em
				_delay_ms(50);
			}
			else if( (dataReceived == COMMAND_PING_OK) && (PingTARGET == SENDERID) ){
				numberPreview(PingTARGET-MASTER);
				PingTARGET = 0;
				USART_Transmit(SENDERID-MASTER); //poslij kompowi co otrzyma�em
				//_delay_ms(50);
				//numberPreview(0);
			}
			else if(ACK_REQUESTED){
				sendFrame(SENDERID, 0, 1, false, true);
			}
		}
		//_delay_ms(50);
		//LED_FALSTART_OFF;
		//sensorPreview(0);
	}
}

int main() {

	init_LED();
	init_USART(MYUBRR);
	init_SPI_master();
	init_RFM69();
	init_RFM_RX_INT();
	sei();
	LED_GO_ON;

	SPI_receiveFlag=false;
	dataFromPC=0;

	/*_delay_ms(200);
	_delay_ms(200);
	_delay_ms(100);
	ping(5);*/

	receiveBegin();
	while(1){

		if( /*(mode != RF69_MODE_RX) &&*/ (SPI_receiveFlag) ){
			_delay_ms(200);
			_delay_ms(200);
			_delay_ms(100);
			receiveBegin();
			SPI_receiveFlag=false;
			sensorPreview(0);
		}
		if(dataFromPC != 0)  dataFromPC=0;
	}
}

