build:
	mkdir -p bin
	echo Using: ${GCC_COMPILER}
	${GCC_COMPILER}-g++ main.cpp -o ./bin/hello

deploy:
	adb push ./bin/hello /oem/hello
