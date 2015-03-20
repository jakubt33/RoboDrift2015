#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "io.h"
#include "init.h"

volatile uint8_t TsalCounter=0;
ISR(TIMER1_COMPA_vect){
	if(TsalCounter<20){
		TSAL_ON;
	}else if(TsalCounter<40){
		TSAL_OFF;
	}else{
		TsalCounter=0;
	}
	TsalCounter++;
}

ISR(INT0_vect){
	_delay_us(10); //wait to be sure
	if(bit_is_set(PIN_TSOP, TSOP)){ 	//TSAL----||----TSOP
		LED1_ON;
		LED2_OFF;
	}
	else{ 								//TSAL -------- TSOP
		LED2_ON;
		LED1_OFF;
	}
}
/*
volatile uint8_t batt_level=0;
ISR(TIMER0_OVF_vect){
	batt_level++;
	//if(batt_level % 2 == 1) LED1_ON;
}*/

int main() {

	init_IO();
	//init_ADC();
	init_CTC();
	init_TSOP();

	sei();

	while(1){

	}
}
