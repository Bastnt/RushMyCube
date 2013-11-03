#pragma once

#include "MiniGame.h"
#include "BlockMatrix.h"

//Hold the time interval between two considerations of movement input while pressing the arrow for a "long" time
#define BUTTON_INTERVAL 60

class PuzzleLeagueLike : public MiniGame {
	private:
		//Matrix
		//---------------------------------------------------------------------------------
		BlockMatrix* _matrix;
		//Remember the initial matrix
		BlockMatrix* _startingMatrix;

		//Graphics
		//---------------------------------------------------------------------------------
		//The ofImage of the background
		ofImage _background;

		//Cursor
		//---------------------------------------------------------------------------------
		//The ofImage of the cursor
		ofImage _cursorImage;
		//The column of the cursor
		char _cursorColumn;
		//The row of the cursor
		char _cursorRow;
		//The elapsed time when the previous movement occured
		unsigned long long _startingMovementTime;

		//Controls
		//---------------------------------------------------------------------------------
		//The Wiimote
		WM* _wiimote;

		//Subfunctions
		//---------------------------------------------------------------------------------
		//Allow movement of the cursor if the elapsed time is correct and return true, else return false
		bool allowMoving();
		//Make the cursor move to the left
		void leftMove();
		//Make the cursor move to the right
		void rightMove();
		//Make the cursor move up
		void upMove(); 
		//Make the cursor move down
		void downMove();

	public:
		//Constructor and destructor
		//---------------------------------------------------------------------------------
		//Default constructor : do nothing
		PuzzleLeagueLike();
		//Create a mini-game of the provided level
		PuzzleLeagueLike(int level);
		//Destructor
		~PuzzleLeagueLike();

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