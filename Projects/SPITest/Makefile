build:
	mkdir -p bin
	echo Using: ${GCC_COMPILER}
	${GCC_COMPILER}-g++ main.cpp -o ./bin/spiTest

deploy:
	adb push ./bin/spiTest /oem/spiTest
