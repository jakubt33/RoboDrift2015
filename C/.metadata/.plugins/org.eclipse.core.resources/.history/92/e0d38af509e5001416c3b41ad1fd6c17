/*
 * main.h
 *
 *  Created on: 7 mar 2015
 *      Author: Kuba
 */

#ifndef IO_H_
#define IO_H_

//------------------------------------
#define DDR_LED DDRC

#define LED1 PC1
#define LED2 PC2
#define LED3 PC3
#define LED4 PC4
#define LED5 PC5

#define LED1_OFF PORTC &= ~_BV(LED1);
#define LED1_ON PORTC |= _BV(LED1);
#define LED2_OFF PORTC &= ~_BV(LED2);
#define LED2_ON PORTC |= _BV(LED2);
#define LED3_OFF PORTC &= ~_BV(LED3);
#define LED3_ON PORTC |= _BV(LED3);
#define LED4_OFF PORTC &= ~_BV(LED4);
#define LED4_ON PORTC |= _BV(LED4);
#define LED5_OFF PORTC &= ~_BV(LED5);
#define LED5_ON PORTC |= _BV(LED5);
//------------------------------------

//------------------------------------
#define DDR_LED_BATT DDRD

#define LED_BATT_GR PD0
#define LED_BATT_RED PD1

#define LED_BATT_GR_OFF PORTD &= ~_BV(LED_BATT_GR);
#define LED_BATT_GR_ON PORTD |= _BV(LED_BATT_GR);
#define LED_BATT_RED_OFF PORTD &= ~_BV(LED_BATT_RED);
#define LED_BATT_RED_ON PORTD |= _BV(LED_BATT_RED);
//------------------------------------

//------------------------------------
#define DDR_TSAL DDRB
#define DDR_STRIPE DDRB
#define PORT_TSAL PORTB
#define PORT_STRIPE DDRB

#define SWITCH_5V PC0
#define DDR_5V DDRC
#define PORT_5V PORTC

#define TSAL_ENABLE PB1
#define LED_STRIPE PB0

#define TSAL_OFF PORT_TSAL &= ~_BV(TSAL_ENABLE);
#define TSAL_ON PORT_TSAL |= _BV(TSAL_ENABLE);
#define LED_STRIPE_OFF PORT_STRIPE &= ~_BV(LED_STRIPE);
#define LED_STRIPE_ON PORT_STRIPE |= _BV(LED_STRIPE);

//------------------------------------

//------------------------------------
#define DDR_SWITCH DDRD
#define PIN_SWITCH PIND

#define SWITCH_UP PD7
#define SWITCH_OK PD6
#define SWITCH_DOWN PD5
//------------------------------------

//------------------------------------
#define DDR_TSOP DDRD
#define PIN_TSOP PIND
#define TSOP PD2
//------------------------------------

//------------------------------------
#define DDR_SPI DDRB
#define PORT_SPI PORTB
#define SS PB2
#define SCK PB5
#define MOSI PB3
#define MISO PB4
#define DIO0 PD3
#define	PORT_DIO PORTD
#define PIN_DIO PIND
#define DDR_DIO DDRD
//------------------------------------

#endif /* IO_H_ */
