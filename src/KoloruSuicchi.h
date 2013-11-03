#pragma once

#include "MiniGame.h"
#include "QuickSprite.h"
#include "WM.h"
using namespace std;

//Hold the number of different colors that can be displayed in highest levels
#define NUMBER_COLORS 17
//Hold the time interval between two considerations of movement input while pressing the arrow for a "long" time
#define BUTTON_INTERVAL 80
//Hold the path for the data (images...)
#define PATH string("KoloruSuicchi/")

//Main structure used in KoloruSuicchi
struct SquareColor {
	//The ofColor of the letters
	ofColor color;
	//The ofColor of the background
	ofColor background;
	//The string which is a color (like "green")
	string name;
};

class KoloruSuicchi : public MiniGame {
	private:		
		//Static
		//---------------------------------------------------------------------------------
		//Available colors, the size is NUMBER_COLORS
		const static ofColor COLORS[];
		//Associated names of the colors of COLORS, same size
		const static string NAMES[]; 
		
		//Matrix
		//---------------------------------------------------------------------------------
		//The matrix containing all the colors informations
		SquareColor** _matrix;
		//The size of the previous matrix
		int _matrixSize;
		//Number of colors used for this instance, which is less or equal to NUMBER_COLORS
		int _availableColors;
		
		//Pause
		//---------------------------------------------------------------------------------
		//The elapsed time when the pause started
		float _pauseStartingTime;

		//Answer
		//---------------------------------------------------------------------------------
		//The level of the game, the higher the harder
		int _level;
		//The line of the answer
		char _answerLine;
		//The column of the answer
		char _answerColumn;
		//True if the game is solved
		bool _solved;
		//True if the rule of the game is to match the text with the background
		bool _matchBackground;

		//Switch
		//---------------------------------------------------------------------------------
		//The length of the switch in seconds
		float _switchLength;
		//The elapsed time when the previous switch occured
		float _switchStart;

		//Controls
		//---------------------------------------------------------------------------------
		//The Wiimote
		WM* _wiimote;

		//Graphics
		//---------------------------------------------------------------------------------
		//The background 
		ofImage _background;
		//The police used to write all the textual informations
		ofTrueTypeFont _visitor;

		//Cursor
		//---------------------------------------------------------------------------------
		//The line of the cursor
		char _cursorLine;
		//The column of the cursor
		char _cursorColumn;
		//The image of the cursor
		ofImage _cursorImage;
		//The red cross which is displayed when fails occur
		QuickSprite _failScreen;
		
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
		//Generate the level randomly
		void levelGenerator();
		//Allow movement of the cursor if the elapsed time is correct and return true, else return false
		
	public:
		//Constructors and destructor
		//---------------------------------------------------------------------------------
		//Default constructor : do nothing
		KoloruSuicchi();
		//Create a mini-game of the provided level, using the levelGenerator method
		KoloruSuicchi(int level);
		//Destructor
		~KoloruSuicchi();

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
