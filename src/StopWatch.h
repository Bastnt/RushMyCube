#pragma once
#include "ofMain.h"

class StopWatch {
	private:
		//Graphics
		//---------------------------------------------------------------------------------
		//The police used to draw the stopwatch
		ofTrueTypeFont _visitor;
		
		//Stopwatch variables
		//---------------------------------------------------------------------------------
		//Starting time
		unsigned long _startingTime;
		//Pausing time
		unsigned long _pausingTime;
		//Used to display the good amount of seconds
		unsigned int _elapsedSeconds;
		//Displayed time
		string _actualTime;
		//True if the stopwatch is enabled
		bool _started;
		//Used to set the images and polices
		ofVec2f _anchor50;

	public:
		//Constructors and destructor
		//---------------------------------------------------------------------------------
		//Default constructor : do nothing
		StopWatch();
		//Destructor
		~StopWatch();
		
		//Subfunctions
		//---------------------------------------------------------------------------------
		//Draw the stopwatch at provided position depending of thz provided size
		void draw(int size, float x, float y);
		//Update the stopwatch
		void update();
		//Start the stopwatch
		void start();
		//Pause the stopwatch
		void pause();
		//Resume the stopwatch
		void resume();
		//Stop the stopwatch
		void stop();
};