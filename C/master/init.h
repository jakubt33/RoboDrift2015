/*
 * init.h
 *
 *  Created on: 18 mar 2015
 *      Author: Kuba
 */

#ifndef INIT_H_
#define INIT_H_

void init_RFM_RX_INT();
uint8_t init_RFM69();
void init_LED();
void init_USART(unsigned char);
void init_SPI_master();
//void init_SPI_slave();

#endif /* INIT_H_ */
