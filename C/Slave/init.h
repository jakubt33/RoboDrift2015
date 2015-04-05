/*
 * init.h
 *
 *  Created on: 17 mar 2015
 *      Author: Kuba
 */

#ifndef INIT_H_
#define INIT_H_

uint8_t check_ADC();
void init_batteries();
void init_IO();
void init_SPI();
void init_TSAL();
void init_TSOP();
void init_SPI_master();

#endif /* INIT_H_ */
