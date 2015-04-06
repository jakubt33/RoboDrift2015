#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "main.h"
#include "io.h"
#include "init.h"
#include "communication.h"
#include "RFM69.h"
#include "RFM69registers.h"

ISR(TIMER1_COMPA_vect){
	if(RaceStart){
		if(TsalGapCounter<BURST_L){//no gap
			//LED5_ON;
			//........................600us tsal is set, 600us tsal is down
			if(TsalCounter<BURST){
				TSAL_ON;
				//LED4_ON;
				TsalCounter++;
			}else if(TsalCounter<BURST+GAP){
				TSAL_OFF;
				//LED4_OFF;
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
			//LED5_OFF;
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
	else{
		TSAL_OFF;
	}
}

ISR(INT0_vect){
	_delay_ms(1); //wait to be sure
	if(bit_is_set(PIN_TSOP, TSOP)){ 	//TSAL----||----TSOP
		;//LED1_ON;
		//LED2_OFF;
	}
	else{ 								//TSAL -------- TSOP
		;//LED2_ON;
		//LED1_OFF;
	}
}

ISR(TIMER2_OVF_vect){
	cli();
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
	sei();
}

int main() {

	init_IO();
	//init_TSAL();
	//init_TSOP();
	init_batteries();
	init_SPI_master();
	init_RFM69();

	_delay_ms(5);
	ADC_level=check_ADC();
	SensorID=0;
	sei();

	uint8_t counter = 0;

	while(1){

		receiveBegin();
		ADC_level=check_ADC();
		_delay_ms(30);
		if(!checkPayload()){
			checkButtons();
		}
	}
}
