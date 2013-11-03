#pragma once
#include "ofMain.h"

#define PATH string("LockScreen/")

class LockScreen {
	private:
		//Graphics
		//---------------------------------------------------------------------------------
		//The ofImage of the locker
		ofImage locker;
		//The police used to display textual informations
		ofTrueTypeFont visitor;
		//The string that displays the remaining time
		string _remainingTime;
		
		//Lock variables
		//---------------------------------------------------------------------------------
		//Time when the lock will end
		unsigned long _endLockTime;
		//Seconds till the end
		int _remainingSeconds;
		//True if the lock is still on (and displayed)
		bool _active;

	public:
		//constructors and destructor
		//---------------------------------------------------------------------------------
		//Default constructor : do nothing
		LockScreen();
		//Destructor
		~LockScreen();
		
		//Subfunctions
		//---------------------------------------------------------------------------------
		//Draw the lock depending of the size of the window
		void draw(int size);
		//Getter of _active attribute
		bool isActive();
		//Launch a lockscreen for an amount of time
		void start(int locktime);
		//Stop the lockscreen before its theorical end
		void stop();
		//Perform as an update for lockscreen, checking time and finishing if needed, but also return true the first time the ending time is reached
		bool isFinished();
};