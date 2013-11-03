#pragma once

#include "MiniGame.h"
#include "QuickSprite.h"
#include "DemoCounter.h"

//Hold the path for the data (images...)
#define PATH string("MemWay/")

class MemWay : public MiniGame {
	private:
		//Enumeration of the directions (N : North, W: West, E: East, S: South, U: Undefined)
		enum DIR {
			N=4, W=3, E=1, S=2, U=0
		};
		
		//Controls
		//---------------------------------------------------------------------------------
		//The Wiimote
		WM* _wiimote;
		
		//Graphics
		//---------------------------------------------------------------------------------
		//The ofImage of the background
		ofImage _background;
		ofImage _squareLeft;
		ofImage _squareRight;
		ofImage _squareUp;
		ofImage _squareDown;
		ofImage _squareCenter;
		//The sprite of the red cross which is displayed while failing
		QuickSprite _redCross;
		
		//Game variables
		//---------------------------------------------------------------------------------
		//Size of the good sequence
		char _sequenceLength;
		//Progression of the player so far
		char _progression;
		//Good sequence that needed to be traveled
		DIR _sequence[64];
		//The starting position
		ofPoint origin;
		
		//Demo
		//---------------------------------------------------------------------------------
		//The DemoCounter object for the demo
		DemoCounter _demonstration;
		//Holds the elapsed time when the player accelerate the demo
		unsigned long _accerelationDate;

	public:
		//Constructors and destructor
		//---------------------------------------------------------------------------------
		//Default constructor : do nothing
		MemWay();
		//Create a mini-game of the provided level
		MemWay(int level);
		//Destructor
		~MemWay();
		
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