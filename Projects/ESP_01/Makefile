luckfox:
	mkdir -p ./bin/software
	echo Using: ${GCC_COMPILER}
	${GCC_COMPILER}-g++ ./software/main.cpp -o ./bin/software/esp_now_driver

deploy:
	adb push ./bin/software/esp_now_driver /oem/esp_now_driver
