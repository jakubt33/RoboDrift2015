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
		LED1_ON;
		LED2_OFF;
	}else if(TsalCounter<40){
		TSAL_OFF;
		LED1_OFF;
		LED2_ON;
	}else{
		TsalCounter=0;
	}
	TsalCounter++;

}

int main() {

	init_IO();
	//init_ADC();
	init_CTC();

	sei();

	while(1){
		if(bit_is_set(PIN_TSOP, TSOP)){
			LED_BATT_GR_OFF;
			LED_BATT_RED_ON;
		}
		else{
			LED_BATT_GR_ON;
			LED_BATT_RED_OFF;
		}
	}
		;
}
