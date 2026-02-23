// 23.02.2026 SPO
// LED Blinking on RPi 4b using WiringPi

#include <wiringPi.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

int main(int argc, char* argv[])
{
	// List of Leds in order
	vector<int> ledPins = { 29, 28, 25, 27, 24, 23, 26, 22, 21, 31, 30, 11, 10, 14, 6, 13, 12, 5, 4, 3, 2, 1, 0, 16, 15, 7, 9, 8 };

	// Blink time in ms
	constexpr int blinktime = 10;

	//Init WiringPi and check if ok
	if (wiringPiSetup() == -1)
	{
		cerr << "Failed to init WiringPi" << endl;;
		return 1;
	};


	// Set all pins as OUTPUT
	for (int pin : ledPins)
	{
		pinMode(pin, OUTPUT);
	}

	cout << "Blinking LEDs connected to GPIO pins" << endl;

	while (true)
	{
		for (int pin : ledPins)
		{
			digitalWrite(pin, HIGH);
			sleep_for(milliseconds(blinktime));
		}

		for (int pin : ledPins)
		{
			digitalWrite(pin, LOW);
			sleep_for(milliseconds(blinktime));
		}
	};
	return 0;
};

