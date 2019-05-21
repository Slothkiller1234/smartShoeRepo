#include "Arduino.h"
#include "smartShoe.h"
#include <string.h>


smart_shoe::smart_shoe_pin(int pin)
{
	pinMode(pin, OUTPUT);
	_pin = pin;
}


smart_shoe::smart_shoe()
{
}

void smart_shoe::say(String variable)
{
	Serial.println(variable);
}


	