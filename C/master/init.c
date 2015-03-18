/*
 * init.c
 *
 *  Created on: 17 mar 2015
 *      Author: Kuba
 */

#include "io.h"
#include <avr/io.h>

void init_LED() {
	DDR_LED |= (1 << LED1);
	DDR_LED |= (1 << LED2);
	DDR_LED |= (1 << LED3);
	DDR_LED |= (1 << LED4);
	DDR_LED |= (1 << LED5);

	DDR_LED_BATT |= (1 << LED_BATT_GR);
	DDR_LED_BATT |= (1 << LED_BATT_RED);

	//DDR_STRIPE |= (1 << LED_STRIPE);
	DDR_TSAL |= (1 <<  TSAL_ENABLE);

	DDR_SWITCH &= ~(1 << SWITCH_DOWN);
	DDR_SWITCH &= ~(1 << SWITCH_OK);
	DDR_SWITCH &= ~(1 << SWITCH_UP);

	DDR_TSOP_IN &= ~(1 << TSOP_IN);

	DDRB |= _BV(LED1);
}
