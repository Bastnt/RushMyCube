#pragma once
#include "ofMain.h"

class RotatorSprite {
	private:
		//Sprite variables
		//---------------------------------------------------------------------------------
		//True if the sprite is displayed
		bool displayed;
		//Holds the ending time, when the sprite will be removed from the screen
		unsigned long _endingTime;
		//The column of the sprite in the matrix
		int _displayX;
		//The row of the sprite in the matrix
		int _displayY;
		//The color of the sprite (0 : red, 1 : green, else : white)
		int _color;
	public:
		//Construtor and destructor
		//---------------------------------------------------------------------------------
		//Default constructor : do nothing
		RotatorSprite();
		//Create an instance of the provided color (0 : red, 1 : green, else : white)
		RotatorSprite(int color); 
		//Destructor
		~RotatorSprite();
		
		//Subfunctions
		//---------------------------------------------------------------------------------
		//Draw the sprite depending of the size of the window
		void draw(int size);
		//Display the sprite for an amount of time in milliseconds at the provided position in the matrix
		void start(int time_ms,short x, short y);
		//Stop the display before its theorical end
		void stop();
		//Getter of displayed attribute
		bool isActive();
		//Perform as an update for the sprite, checking time and finishing if needed, but also return true the first time the ending time is reached
		bool isFinished();
};