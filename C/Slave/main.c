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

	_delay_ms(20);
	ADC_level=check_ADC();
	SensorID=0;
	sei();

	//uint8_t sensor=1;
	uint8_t counter = 0;
	uint8_t dataReceived=0;


	while(1){

		receiveBegin();
		ADC_level=check_ADC();
		_delay_ms(30);
		if ( (mode == RF69_MODE_RX) && (readReg(REG_IRQFLAGS2) & RF_IRQFLAGS2_PAYLOADREADY) ) {
			//showID(2);
		    setMode(RF69_MODE_STANDBY);
		    select();
		    spi_send(REG_FIFO & 0x7F);
		    //PAYLOADLEN = spi_send(0);
		    //PAYLOADLEN = PAYLOADLEN > 66 ? 66 : PAYLOADLEN; // precaution
		    TARGETID = spi_send(0);

			//DATALEN = PAYLOADLEN - 4;
			SENDERID = spi_send(0);
			uint8_t CTLbyte = spi_send(0);

			ACK_RECEIVED = CTLbyte & 0x80; // extract ACK-received flag
			ACK_REQUESTED = CTLbyte & 0x40; // extract ACK-requested flag

			dataReceived = spi_send(0);
			//for (counter = 0; counter < DATALEN; counter++){
				//DATA[counter] = spi_send(0);
			//}
			//if (DATALEN < RF69_MAX_DATA_LEN) DATA[DATALEN] = 0; // add null at end of string */
			unselect();
		    if(dataReceived == '1'){
		    	showID(1);
		    }
		    else if(dataReceived=='2'){
		    	showID(2);
		    }
		    else if(SENDERID==50){
		    	showID(3);
		    }
		    else if(TARGETID==51){
		    	showID(4);
		    }
		    else if(dataReceived<56){
		    	showID(5);
		    }
		    else showID(6);

		    if(!(promiscuousMode || TARGETID == NODEID || TARGETID == RF69_BROADCAST_ADDR) // match this node's address, or broadcast address or anything in promiscuous mode
		       || PAYLOADLEN < 3) { // address situation could receive packets that are malformed and don't fit this libraries extra fields
		    	//showID(1);
		        PAYLOADLEN = 0;
		        unselect();
		        receiveBegin();
		    }
		    else {
		    	/*
				DATALEN = PAYLOADLEN - 3;
				SENDERID = spi_send(0);
				uint8_t CTLbyte = spi_send(0);

				ACK_RECEIVED = CTLbyte & 0x80; // extract ACK-received flag
				ACK_REQUESTED = CTLbyte & 0x40; // extract ACK-requested flag

				for (uint8_t i = 0; i < DATALEN; i++){
					DATA[i] = spi_send(0);
				}
				if (DATALEN < RF69_MAX_DATA_LEN) DATA[DATALEN] = 0; // add null at end of string */
				unselect();
			}
			//RSSI = readRSSI(); */
		    writeReg(REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN);
		}
		//else showID(1);
	}


		/*
		if(SPSR & (1<<SPIF)){ //if message received
			switch(SPDR){
			case 1:
				RaceStart=1;//race on
				SPDR=5;
				break;
			case 2:
				RaceStart=0;//race off
				SPDR = 3;
				break;
			default:
				break;
			}
		}
		if(RaceStart){

		}
		else{
			sensor=1;
			checkButtons();
		}*/

		//.............................SPI
		/*while(!(SPSR & (1<<SPIF)));
		switch(SPDR){
		case 1:
			RaceStart=1;//race on
			SPDR=5;
			break;
		case 2:
			RaceStart=0;//race off
			SPDR = 4;
			break;
		default:
			SPDR=3;
			break;
		}*/
		//..............................SPI
}
