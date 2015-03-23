#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "io.h"
#include "init.h"

volatile uint8_t TsalCounter=0;
volatile uint8_t TsalGapCounter=0;
#define BURST 7 //1 BURST~=103us
#define GAP 7	//1 GAP~=103us
#define BURST_L 3 //1 BURST_L=(BURST+GAP)*103us
#define GAP_L 3
ISR(TIMER1_COMPA_vect){
	if(TsalGapCounter<BURST_L){//no gap
		LED5_ON;
		//........................600us tsal is set, 600us tsal is down
		if(TsalCounter<BURST){
			TSAL_ON;
			LED4_ON;
			TsalCounter++;
		}else if(TsalCounter<BURST+GAP){
			TSAL_OFF;
			LED4_OFF;
			TsalCounter++;
			if(TsalCounter>=BURST+GAP){
				TsalCounter=0;
				TsalGapCounter++;
				if(TsalGapCounter>=BURST_L+GAP_L){
					TsalGapCounter=0;
				}
			}
		}
		//........................600us tsal is set, 600us tsal is down
	}
	else if(TsalGapCounter<BURST_L+GAP_L){//gap
		LED5_OFF;
		TSAL_OFF;
		if(TsalCounter<BURST){
			TsalCounter++;
		}else if(TsalCounter<BURST+GAP){
			TsalCounter++;
			if(TsalCounter>=BURST+GAP){
				TsalGapCounter++;
				TsalCounter=0;
				if(TsalGapCounter>=BURST_L+GAP_L){
					TsalGapCounter=0;
				}
			}
		}

	}
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
volatile uint8_t ADC_level=0;
#define ADC_min 135
#define ADC_max 172
ISR(TIMER2_OVF_vect){
	if( ovf < (ADC_level-ADC_min+4) ) { //getting result from 4 to 42;
		LED_BATT_GR_OFF;
		LED_BATT_RED_OFF;
	}
	else {
		if(ovf >= 4){
			LED_BATT_GR_ON;
		}
		else{
			LED_BATT_GR_OFF;
			LED_BATT_RED_ON;
		}
		ovf=0;
	}
	ovf++;
}

int main() {

	init_IO();
	init_TSAL();
	init_TSOP();
	init_batteries();

	ADC_level=check_ADC();
	sei();

	while(1){
		;
	}
}
