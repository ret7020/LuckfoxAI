build:
	echo Using: ${SDK}
	${SDK}-g++ main.cpp -o ./bin/sim800_test

deploy:
	adb push ./bin/sim800_test /oem/sim800_test
