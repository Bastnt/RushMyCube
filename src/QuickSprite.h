#pragma once
#include "ofMain.h"

class QuickSprite {
	private:
		//Sprite variables
		//---------------------------------------------------------------------------------
		//True if the sprite is displayed
		bool _displayed;
		//Holds the ending time, when the sprite will be removed from the screen
		unsigned long _endingTime;
		
		//Graphics
		//---------------------------------------------------------------------------------
		//The ofImage of the red cross
		ofImage _redCross;
		//The background color when the cross is displayed (in hexadecimal), -1 will perform an uncolored and transparent background
		int _backgroundColor;

	public:
		//Construtor and destructor
		//---------------------------------------------------------------------------------
		//Default constructor : do (almost) nothing
		QuickSprite();
		//Destructor
		~QuickSprite();
		
		//Subfunctions
		//---------------------------------------------------------------------------------
		//Initialization method, load image and set it and the background
		void init(string path, int backgroundColor = -1);
		//Draw the sprite depending of the size of the window
		void draw(int size);
		//Display the sprite for an amount of time in milliseconds
		void start(int time_ms);
		//Stop the display before its theorical end
		void stop();
		//Getter of displayed attribute
		bool isActive();
		//Perform as an update for the sprite, checking time and finishing if needed, but also return true the first time the ending time is reached
		bool isFinished();
};