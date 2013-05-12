all:
	g++ main.cpp sensor.cpp file.cpp atod.cpp -I/usr/local/include -L/usr/local/lib -lwiringPi -o DataLog