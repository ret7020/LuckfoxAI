build:
	mkdir -p bin
	echo Using: ${GCC_COMPILER}
	${GCC_COMPILER}-g++ main.cpp -o ./bin/setPin

deploy:
	adb push ./bin/setPin /oem/setPin
