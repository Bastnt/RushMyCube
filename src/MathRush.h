#pragma once

#include "MiniGame.h"
#include "QuickSprite.h"

//Hold the path for the data (images...)
#define PATH string("MathRush/")

class MathRush : public MiniGame {
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
		//The sprite of the red cross which is displayed while failing
		QuickSprite _redCross;
		//The police used to write the numbers
		ofTrueTypeFont _visitor;
		
		//Game variables
		//---------------------------------------------------------------------------------
		//Starting position in the matrix
		ofVec2f _origin;
		//Current position in the matrix
		ofVec2f _current;
		//Matrix
		int _matrix[8][8];
		//The value displayed in the top that needed to be added or substracted when moving
		int _next;
		//Hold the incremental value so far (value of the starting case + _next * number of moves)
		int _currentTotal;
		//Size of the good sequence
		char _sequenceLength;
		//Progression of the player so far
		char _progression;
		//Good sequence that needed to be traveled
		DIR _sequence[64];

	public:
		//Constructors and destructor
		//---------------------------------------------------------------------------------
		//Default constructor : do nothing
		MathRush();
		//Create a mini-game of the provided level
		MathRush(int level);
		//Destructor
		~MathRush();
		
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