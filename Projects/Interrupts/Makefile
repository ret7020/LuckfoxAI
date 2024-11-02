build:
	mkdir -p bin
	echo Using: ${GCC_COMPILER}
	${GCC_COMPILER}-g++ main.cpp -o ./bin/handle

deploy:
	adb push ./bin/handle /oem/handle
