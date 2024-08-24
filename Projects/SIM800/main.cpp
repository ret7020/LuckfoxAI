#include <stdio.h>
#include "sim800.h"

// Config

#define MODULE_UART "/dev/ttyS4"
#define MODULE_APN "INTERNET.MTS.RU"


int main()
{
	SIM800 module = SIM800(MODULE_UART);
	int initStatus = module.init();
	printf("Init status: %d\n", initStatus);
	if (initStatus)
	{
		char command[] = "AT\r";
		char rxBuf[2];
		module.sendCommand(command, rxBuf);
		//module.setupInternet();
	
	} else return 1;
}
