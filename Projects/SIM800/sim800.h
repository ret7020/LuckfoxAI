#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <charconv>
#include <string>


#define WORKING_BAUD B115200
#define MAX_RX_BYTES 2048

class SIM800
{
private:
	int uartFd;
	const char* port;
	struct termios tty;
	ssize_t bytesWritten;
	int bytesRead;
	char rxBuf[MAX_RX_BYTES];

public:
	bool initStatus = false;

	SIM800(const char* port)
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

	bool writeRaw(const char* data)
	{
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

	int sendCommand(const char* command, char* answerBuffer)
	{
		if (writeRaw(command)){
			usleep(500000);
			return readRaw(answerBuffer);
		}
		return 0;
	}

	bool checkAT()
	{
		if (initStatus)
		{

			if (sendCommand("AT\r", rxBuf)){
				if (rxBuf[0] == 'O' && rxBuf[1] == 'K' ) return true;
			}
				
		}
		return false;
	}

	void setupInternet(const char* apn)
	{
		if (initStatus){
			sendCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r", rxBuf);
			char apnSetupCommand[sizeof(apn) + 25];
			strcpy(apnSetupCommand, "AT+SAPBR=3,1,\"APN\",\"");
			strcat(apnSetupCommand, apn);
			strcat(apnSetupCommand, "\"\r");

			sendCommand(apnSetupCommand, rxBuf);
			sendCommand("AT+SAPBR=1,1\r", rxBuf);
			sendCommand("AT+SAPBR=2,1\r", rxBuf);
			sendCommand("AT+HTTPINIT\r", rxBuf);
			sendCommand("AT+HTTPPARA=\"CID\",1\r", rxBuf);
		}
	}

	void finishInternet()
	{
		if (initStatus)
		{
			sendCommand("AT+HTTPTERM\r", rxBuf);
			sendCommand("AT+SAPBR=0,1\r", rxBuf);
		}
	}

	// HTTP requests
	
	void get(const char* url, char* response)
	{
		if (initStatus)
		{
			char setUrlCommand[sizeof(url) + 25];
			strcpy(setUrlCommand, "AT+HTTPPARA=\"URL\",\"");
			strcat(setUrlCommand, url);
			strcat(setUrlCommand, "\"\r");
			sendCommand(setUrlCommand, rxBuf);

			sendCommand("AT+HTTPACTION=0\r", rxBuf);
			sleep(2);
			sendCommand("AT+HTTPREAD\r", response);
		}
	}

	void post(const char* url, char* response, char* data, int latency=10000)
	{
		if (initStatus)
		{
			char setUrlCommand[sizeof(url) + 25];
			strcpy(setUrlCommand, "AT+HTTPPARA=\"URL\",\"");
			strcat(setUrlCommand, url);
			strcat(setUrlCommand, "\"\r");
			sendCommand(setUrlCommand, rxBuf);
			char setDataParams[70];
			strcpy(setDataParams, "AT+HTTPDATA=");
			strcat(setDataParams, std::to_string(sizeof(data)).c_str());
			strcat(setDataParams, ",");
			strcat(setDataParams, std::to_string(latency).c_str());
			sendCommand(setDataParams, rxBuf);

			// Set post data
			char setDataCommand[sizeof(data) + 1];
			strcpy(setDataCommand, data);
			strcat(setDataCommand, "\r");
			sendCommand(setDataCommand, rxBuf);
			sendCommand("AT+HTTPACTION=1\r", rxBuf);
			readRaw(response);
		}
	}
	
};
