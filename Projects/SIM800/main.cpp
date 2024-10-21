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
		if (module.checkAT())
		{
			module.setupInternet(MODULE_APN);
			char response[2000];
			module.get("https://httpbin.org/get", response);
			printf("Response: %s", response);

		}
		
	
	} else return 1;

	module.finishInternet();
	return 0;
}
