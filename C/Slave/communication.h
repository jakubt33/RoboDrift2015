/*
 * communication.h
 *
 *  Created on: 19 mar 2015
 *      Author: Kuba
 */

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#define COMMAND_PING 0b00110011
#define COMMAND_PING_OK 0b00111111
#define COMMAND_PING_NO_OK 0b00110000

uint8_t spi_send(uint8_t);
void showID(uint8_t);
void checkButtons();
uint8_t checkPayload();
#endif /* COMMUNICATION_H_ */
