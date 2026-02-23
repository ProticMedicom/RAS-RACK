#include <wiringPi.h>
#include <iostream>
#include <chrono>
#include <thread>

constexpr int LED_PIN_26 = 25;
constexpr int LED_PIN_19 = 24;
constexpr int LED_PIN_13 = 23;

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

int main(int argc, char* argv[])
{

	//Init WiringPi and check if ok
	if (wiringPiSetup() == -1)
	{
		cerr << "Failed to init WiringPi" << endl;;
		return 1;
	};

	pinMode(LED_PIN_26, OUTPUT);		// Set LED pin @% as OUT
	pinMode(LED_PIN_19, OUTPUT);		// Set LED pin @% as OUT
	pinMode(LED_PIN_13, OUTPUT);		// Set LED pin @% as OUT

	cout << "Blinking LED connected to GPIO pins" << endl;

	while (true)
	{

		digitalWrite(LED_PIN_26, HIGH);
		sleep_for(milliseconds(500));

		digitalWrite(LED_PIN_19, HIGH);
		sleep_for(milliseconds(500));

		digitalWrite(LED_PIN_13, HIGH);
		sleep_for(milliseconds(500));


		digitalWrite(LED_PIN_26, LOW);
		sleep_for(milliseconds(500));

		digitalWrite(LED_PIN_19, LOW);
		sleep_for(milliseconds(500));

		digitalWrite(LED_PIN_13, LOW);
		sleep_for(milliseconds(500));

	};
	return 0;
};

