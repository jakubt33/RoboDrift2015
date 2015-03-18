/*
 * IO.h
 *
 *  Created on: 17 mar 2015
 *      Author: Kuba
 */

#ifndef IO_H_
#define IO_H_

//------------------------------------
#define DDR_LED DDRD

#define LED1S PD0
#define LED2S PD1
#define LED3S PD5
#define LED4S PD6
#define LED5S PD7

#define LED1S_OFF PORTC &= ~_BV(LED1S);
#define LED1S_ON PORTC |= _BV(LED1S);
#define LED2S_OFF PORTC &= ~_BV(LED2S);
#define LED2S_ON PORTC |= _BV(LED2S);
#define LED3S_OFF PORTC &= ~_BV(LED3S);
#define LED3S_ON PORTC |= _BV(LED3S);
#define LED4S_OFF PORTC &= ~_BV(LED4S);
#define LED4S_ON PORTC |= _BV(LED4S);
#define LED5S_OFF PORTC &= ~_BV(LED5S);
#define LED5S_ON PORTC |= _BV(LED5S);
//------------------------------------


#endif /* IO_H_ */
