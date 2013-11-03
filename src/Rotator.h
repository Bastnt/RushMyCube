#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MiniGame.h"
#include "RotatorSprite.h"
#include "DemoCounter.h"
using namespace std;

//Hold the time length of the white circle display
#define DEMO_ANIMATION_LENGTH 700
//Hold the time length of the red and green circles display
#define ANIMATION_LENGTH 400
//Hold the time interval between two white circles during the demo
#define DEMO_ANIMATION_INTERVAL 300
//Hold the time interval between two considerations of movement input while pressing the arrow for a "long" time
#define BUTTON_INTERVAL 0.13f
//Hold the path for the data (images...)
#define PATH string("Rotator/")

//Structure used to reduce the number of variables used to remember positions in the matrix
struct Position {
	short X;
	short Y;
};

class Rotator : public MiniGame {
	private:
		//game variables
		//---------------------------------------------------------------------------------
		//The level of the game, the higher the harder
		int _level;

		//Controls
		//---------------------------------------------------------------------------------
		//The Wiimote
		WM* _wiimote;

		//Answer and progression
		//---------------------------------------------------------------------------------
		//Length of the sequence that need to be reproduced
		int _sequenceLength;
		//Current position of the player in the previous sequence
		int _progression;
		//Sequence that need to be reproduced
		Position* _sequence;

		//Graphics
		//---------------------------------------------------------------------------------		
		//The value of the rotation of the matrix
		float _rotation;
		//The ofImage of the background
		ofImage _background;
		//The ofImage of the compass on the top-right corner
		ofImage _compass;
		//The changing ofColor of the background
		ofColor _backgroundColor;
		//The sprite of the red circle
		RotatorSprite* _redCircle;
		//The sprite of the green circle
		RotatorSprite* _greenCircle;
		//The sprite of the white circle
		RotatorSprite* _whiteCircle; 
		//Value used to update _rotation value
		unsigned long _rotateTime;
		//The ofImage of the cursor
		ofImage _cursorImage;

		//Demo
		//---------------------------------------------------------------------------------
		//Current position in the demo
		short _demoIndex;
		//DemoCounter object for the demo
		DemoCounter* _demo;

		//Cursor
		//---------------------------------------------------------------------------------
		//Current Position of the cursor
		Position _cursor;
		
		//Subfunctions
		//---------------------------------------------------------------------------------
		//Make the cursor move to the left
		void leftMove();
		//Make the cursor move to the right
		void rightMove();
		//Make the cursor move up
		void upMove(); 
		//Make the cursor move down
		void downMove();
		//Change the color of the background
		void gradientTransition(float animationLength);
		//Return the ofColor between the two provided (with the provided progression)
		ofColor colorTransition(const ofColor& c1,const ofColor& c2, float progression);
		//Launch a demo
		void launchDemo();

		//Pause
		//---------------------------------------------------------------------------------
		//The elapsed time when the pause started
		unsigned long _pauseStartingTime;
	public:
		//Constructors and destructor
		//---------------------------------------------------------------------------------
		//Default constructor : do nothing
		Rotator();
		//Create a mini-game of the provided level
		Rotator(int level);
		//Destructor
		~Rotator();

		//Inheritance
		//---------------------------------------------------------------------------------
		void setup(WM* wiimote);
		void update();
		void draw(int size);
		bool solved();
		void pause();
		void resume();
		MiniGame* clone();
};