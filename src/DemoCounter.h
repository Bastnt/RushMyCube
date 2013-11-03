#pragma once
#include "ofMain.h"

class DemoCounter {
	private:
		//True if the demonstration has started
		bool _started;
		//The length of a time division
		float _timeDivision;
		//Remember the the ending time
		unsigned long _endingTime;
		//Elapsed time when the pause has started
		unsigned long _pausingTime;
		//Current top value
		unsigned int _currentTop;
		//The amount of time after the demo ends
		unsigned int _delayAfter;
		//The amount of time before the demo starts
		unsigned int _delayBefore;

	public:
		//Default constructor : do (almost) nothing
		DemoCounter();
		//Launch a demo for an amount of time an with an amount of tops (with eventual delays)
		void start(int time, int tops, int delay_before=0, int delay_after=0);
		//Pause the demo
		void pause();
		//Resume the demo
		void resume();
		//True if the demo is active
		bool isActive();
		//Stop the demo
		void stop();
		//Getter of _currentTop attribute
		unsigned int getCurrentTop();
		//Return true if a new top has occured
		bool newTop();
		//Return true the first time the function is called after the end of the demo and stop it, else return false
		bool isFinished();
		//Accelerate the demo 
		void accelerate();
};