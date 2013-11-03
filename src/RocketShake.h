#pragma once

#include "MiniGame.h"

//Hold the path for the data (images...)
#define PATH string("RocketShake/")

class RocketShake : public MiniGame {
	private:		
		//Game variables
		//---------------------------------------------------------------------------------
		//The height of the displayed rocket
		float _rocketHeight;
		//The speed of the rocket
		float _speed;
		//The level of the game
		short _level;
		//Hold the elapsed time to update each time it is required
		unsigned long _date;
		
		//Controls
		//---------------------------------------------------------------------------------
		//The Wiimote
		WM* _wiimote;
		
		//Graphics
		//---------------------------------------------------------------------------------
		//The ofImage of the background
		ofImage _background;
		//The ofImage of the first planet in the background
		ofImage _planet1;
		//The ofImage of the second planet in the background
		ofImage _planet2;
		//The ofImage of the moon in the background
		ofImage _moon;
		//The ofImage of the fire in the foreground
		ofImage _fire;
		//The ofImage of the rocket in the foreground
		ofImage _rocket;
		//The position of the first planet in the background
		ofVec2f _positionPlanet1;
		//The position of the second planet in the background
		ofVec2f _positionPlanet2;

	public:
		//Constructors and destructor
		//---------------------------------------------------------------------------------
		//Default constructor : do nothing.
		RocketShake();
		//Create a mini-game of the provided level
		RocketShake(int level);
		//Destructor
		~RocketShake();
		
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