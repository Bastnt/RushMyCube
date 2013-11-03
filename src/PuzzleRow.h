#pragma once
#include "ofMain.h"

//Hold the path for the data (images...)
#define PATH string("PuzzleLeagueLike/")

//Enumeration of the possible colors, empty is a special case
enum ColorBlock {empty, yellow, blue, green, red, purple};

//Main structure used in the rows, represents a block
struct Block {
	//The color of the block
	ColorBlock color;
	//True if the block is locked
	bool locked;
	//True if the block is animated (different from locked)
	bool animated;
	//Value of the animation of the block, used to draw
	char animationPercent;
};

class PuzzleRow {	
	private:
		//Static
		//---------------------------------------------------------------------------------
		//The ofImage of the yellow blocks
		static ofImage _yellow;
		//The ofImage of the blue blocks
		static ofImage _blue;
		//The ofImage of the green blocks
		static ofImage _green;
		//The ofImage of the red blocks
		static ofImage _red;
		//The ofImage of the purple blocks
		static ofImage _purple;
		//True if the images are already loaded and set
		static bool _imagesLoaded;
	public:
		//Blocks
		//---------------------------------------------------------------------------------
		Block _blocks[6];
		
		//Constructors and destructor
		//---------------------------------------------------------------------------------
		//Default constructor : do nothing
		PuzzleRow();
		//Copy constructor
		PuzzleRow(const PuzzleRow& toCopy);
		//Destructor
		~PuzzleRow();

		//Subfunctions
		//---------------------------------------------------------------------------------
		//Fill the row with allowed values : no triplet or more in the row
		void filledRow();
		//Fill the row with empty values
		void emptyRow();
		//True if the row is empty
		bool isEmpty();
		//Perform an exchange of Block between the column of the right and the provided one
		void exchange(int column);
		//Draw the row of the provided size (with intervals)
		void draw(int rowSize);
		//True if a triplet is found by putting the two provided rows on top of the current one
		bool hasVerticalTriplet(PuzzleRow& p1, PuzzleRow& p2);
		//Load the images and set the anchor percent of each of them
		static void initImages();
};