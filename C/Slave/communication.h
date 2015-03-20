/*
 * communication.h
 *
 *  Created on: 19 mar 2015
 *      Author: Kuba
 */

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

void spi_send(char);
ISR(SPI_STC_vect);

#endif /* COMMUNICATION_H_ */
