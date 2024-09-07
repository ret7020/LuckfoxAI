build:
	mkdir -p bin
	echo Using: ${GCC_COMPILER}
	${GCC_COMPILER}-g++ main.cpp -o ./bin/uartPong

deploy:
	adb push ./bin/uartPong /oem/uartPong
