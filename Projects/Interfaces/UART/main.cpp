#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define UART_PATH "/dev/ttyS4"

int main()
{
	// Init part
	char serialPort[] = UART_PATH;
	char txBuf[] = "ping\n";
	struct termios tty;
	ssize_t writeLen;
	int serialFd;
	char rxBuffer[256];
	int bytesRead;

	serialFd = open(serialPort, O_RDWR | O_NOCTTY);

	memset(&tty, 0, sizeof(tty));
	

	// Setting baud
	cfsetospeed(&tty, B9600);
	cfsetispeed(&tty, B9600);

	// Generic flags
	tty.c_cflag &= ~PARENB;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;
	
	while (1){
		writeLen = write(serialFd, txBuf, sizeof(txBuf));
		if (writeLen > 0)
		{
			bytesRead = read(serialFd, rxBuffer, sizeof(rxBuffer));
			if (bytesRead > 0) {
				rxBuffer[bytesRead] = '\0';
				printf("Recieved: %s", rxBuffer);
				if (rxBuffer[0] == 'e') {
					printf("Exit\n");
					return 0;
				}
			}
		}
	}

	return 0;
}
