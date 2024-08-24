#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>


#define WORKING_BAUD B115200

class SIM900
{
private:
	int uartFd;
	const char* port;
	struct termios tty;
	ssize_t bytesWritten;
	int bytesRead;

public:
	bool initStatus = false;

	SIM900(const char* port)
	{
		this->port = port;
	}

	bool init()
	// Return 1 if ok; return 0 if error
	{
		printf("Opening %s\n", port);
		uartFd = open(port, O_RDWR | O_NOCTTY);
		if (uartFd != -1) {
			struct termios tty;
			memset(&tty, 0, sizeof(tty));
			if (!tcgetattr(uartFd, &tty))
			{
				cfsetospeed(&tty, WORKING_BAUD);
				cfsetispeed(&tty, WORKING_BAUD);
				tty.c_cflag &= ~PARENB;
				tty.c_cflag &= ~CSTOPB;
				tty.c_cflag &= ~CSIZE;
				tty.c_cflag |= CS8;

				if (!tcsetattr(uartFd, TCSANOW, &tty)) initStatus = true;
			}
		}

		return initStatus;
	}

	bool writeRaw(char* data)
	{
		// char txBuff[MAX_COMMAND_LENGTH];
		// strcpy(txBuff, data);
		bytesWritten = write(uartFd, data, sizeof(data));
		if (bytesWritten > 0) return 1;
		return 0;
	}

	int readRaw(char* data)
	{
		bytesRead = read(uartFd, data, sizeof(data));
		data[bytesRead] = '\0';
		return bytesRead;
	}

	void sendCommand(char* command, char* rxBuff)
	{
		printf("Sending command: %s\n", command);
		if (writeRaw(command))
		{
			int rxCnt = readRaw(rxBuff);
			printf("Recieved %d bytes", rxCnt);
		}
	}

	int checkAT()
	{
		if (initStatus)
		{
			return true;
		}
		return false;
	}

	void setupInternet(){}

};
