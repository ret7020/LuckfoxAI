build:
	echo Using: ${GCC_COMPILER}
	${GCC_COMPILER}-g++ main.cpp -o ./bin/sim800_test

deploy:
	adb push ./bin/sim800_test /oem/sim800_test
