/*
 * main.h
 *
 *  Created on: 2 kwi 2015
 *      Author: Kuba
 */

#ifndef MAIN_H_
#define MAIN_H_

volatile uint8_t SensorID;
volatile uint8_t RaceStart; //race off


//TSAL handler
volatile uint8_t TsalCounter;
volatile uint8_t TsalGapCounter;


//ADC
volatile uint8_t ovf;
volatile uint8_t ADC_level;
#define ADC_min 135
#define ADC_max 172


#endif /* MAIN_H_ */
