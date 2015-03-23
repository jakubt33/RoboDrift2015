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

#include "communication.h"
#include "io.h"
#include "init.h"

volatile unsigned char dataFromPC=0;
ISR(USART_RXC_vect){
	cli();
    dataFromPC=UDR;
    if(dataFromPC&0b10000000){ //if 0b1xxx xxxx  //ping
    	LED_RED_ON;
    	ping(0b00001111&dataFromPC); //get how many sensors to ping
    }
    else if(dataFromPC&0b01000000){ //if 0bx1xx xxxx  //countdown
    	LED_RED_OFF;
    	startCounter(dataFromPC&0b00001111);
    }
    else if(dataFromPC&0b00100000){ //stop
    	stopRace();//disable sensors()
    }
    else if(dataFromPC&0b00010000){ //next sensor
    	;//next sensor()
    }
    dataFromPC=0;
    sei();
}

int main() {

	init_LED();
	init_USART(MYUBRR);
	init_SPI();
	sei();

	unsigned char data=0;
	uint8_t counter=1;
	while(1){
		if(raceFlag){
			_delay_ms(200);
			_delay_ms(200);
			data = (unsigned char)counter;
			USART_Transmit(data);
			counter++;
			if(counter>5) counter=1;
		}
	}
}

