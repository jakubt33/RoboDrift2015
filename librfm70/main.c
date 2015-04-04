/*
	Diese Library bassiert auf der Arduino RFM70 Library, ich habe sie lediglich so
	angepasst, dass man sie auf einem AVR und ohne zusätzliche Libs von Arduino
	verwenden kann.

	Es sind nur die wichtigstens Funktionen implementiert!
	Bei Fragen oder Anregungen:
	Mein Blog: http:://projects.web4clans.com
	Meine Email: daniel.weber@web4clans.com

*/

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



#include "uart.h"
#include "rfm70.h"

#define UART_BAUD_RATE 9600

int main()
{
	//uint8_t test[20]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x78,0x06,0x11,0x99};
	uint8_t buffer[32];

	sei();
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );

	DDRA |= (1<<PA4);
	_delay_ms(1000);
	uart_puts("System Ready\n");
	Begin();

	setMode(1);
	setChannel(8);




	while (1)
	{
	/*
		sendPayload(test,20, 0);
		_delay_ms(20);
	*/	

		if (receivePayload(buffer))
		{
			uart_puts((char*)buffer);
			uart_puts("\n");
		}
	
	}
}
