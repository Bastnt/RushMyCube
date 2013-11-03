#pragma once

#include "PuzzleRow.h"

class BlockMatrix { 
	private: 
		//Game variables
		//---------------------------------------------------------------------------------
		//The height that is needed to be reached in order to win
		int _victoriousHeight;
		
		//Refresh
		//---------------------------------------------------------------------------------
		//Hold a value used to increment the animationPercent
		unsigned long long elapsedTime;
		
		//Subfunctions
		//---------------------------------------------------------------------------------
		//Create a random level
		void levelGenerator();
		//Erase allowed blocks around the provided position
		void erase(int row, int column);
		//Enable gravity on cube that can go down
		void gravity(int row, int column);
		//End or continue the gravity effect of the provided position
		void endGravity(int row,int column);
		
	public: 
		//Constructors and destructor
		//---------------------------------------------------------------------------------
		//Default constructor : do nothing
		BlockMatrix();
		//Create a instance with the provided level
		BlockMatrix(int level);
		//Copy constructor
		BlockMatrix(const BlockMatrix& toCopy);
		//Destructor
		~BlockMatrix();

		//Globals
		//---------------------------------------------------------------------------------
		//The number of rows that can be displayed
		const static int MAX_HEIGHT = 10;
		//The number of rows that are created at the beginning
		const static int STARTING_HEIGHT = 8;
		
		//Matrix
		//---------------------------------------------------------------------------------
		PuzzleRow* _rows[MAX_HEIGHT];
		
		//Subfunctions
		//---------------------------------------------------------------------------------
		//Initializion function
		void setup();
		//Draw the matrix
		void draw(int size);
		//Update the matrix state
		void update();
		//True if the matrix is solved, meaning that the victoriousHeight is reached
		bool solved();
		//Exchange the block of provided position with the column of the right
		void exchange(int row, int column);
		//Insert a new line if the MAX_HEIGHT is not reached
		void newLine();
		//Change the color of the position to empty
		void makeEmpty(int row, int column);
};