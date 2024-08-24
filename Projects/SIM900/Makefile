build:
	echo Using: ${SDK}
	${SDK}-g++ main.cpp -o ./bin/sim900_test

deploy:
	adb push ./bin/sim900_test /oem/sim900_test
