#ifndef smart_shoe_h
#define smart_shoe_h

#include "Arduino.h"


class smart_shoe
{
	public:
		
		smart_shoe();
		smart_shoe_pin(int pin);
		void say(String variable);
	private:
		int _pin;
		char _variable;
};


		

#endif