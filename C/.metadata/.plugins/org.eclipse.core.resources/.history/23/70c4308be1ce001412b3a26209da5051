#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "io.h"
#include "init.h"


int main() {

	init_LED();
	init_ADC();

	while(1) {
		_delay_ms(20);
		uint8_t adc=check_ADC();
		if( adc < 50){
			LED1_ON;
			LED2_ON;
			LED3_OFF;
			LED4_OFF;
			LED5_OFF;
		}
		else if( adc < 100){
			LED1_OFF;
			LED2_ON;
			LED3_OFF;
			LED4_OFF;
			LED5_OFF;
		}
		else if( adc < 150){
			LED1_OFF;
			LED2_OFF;
			LED3_ON;
			LED4_OFF;
			LED5_OFF;
		}
		else if( adc < 200){
			LED1_OFF;
			LED2_OFF;
			LED3_OFF;
			LED4_ON;
			LED5_OFF;
		}
		else{
			LED1_OFF;
			LED2_OFF;
			LED3_OFF;
			LED4_OFF;
			LED5_ON;
		}
	}

}
