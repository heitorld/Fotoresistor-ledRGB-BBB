all:
	arm-linux-gnueabihf-g++ -std=c++11 -o exe main.cpp BlackCore.cpp BlackGPIO/BlackGPIO.cpp -static
