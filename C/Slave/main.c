#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "io.h"
#include "init.h"

volatile uint8_t TsalCounter=0;
ISR(TIMER1_COMPA_vect){ //600us tsal is set, 600us tsal is down
	if(TsalCounter<20){
		TSAL_ON;
		LED4_ON;
	}else if(TsalCounter<40){
		TSAL_OFF;
		LED4_OFF;
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

volatile uint8_t ovf=0;
ISR(TIMER2_OVF_vect){
	if(ovf<3) {
		LED3_ON;
	}
	else if(ovf<5){
		LED3_OFF;
	}
	else{
		ovf=0;
	}
	ovf++;
}

int main() {

	init_IO();
	init_TSAL();
	_delay_ms(1);
	init_TSOP();
	_delay_ms(1);
	init_batteries();

	sei();

	while(1){

	}
}
