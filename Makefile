all:
	g++ src/CppRPiGPIO/CppRPiGPIO.cpp src/FanControl.cpp -o install/FanControl

clean: 
	rm install/FanControl
