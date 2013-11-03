#pragma once

#include "ofMain.h"
#include "WM.h"

//Abstract class implemented by all the mini-games
class MiniGame {
	public:
		//Cloning method, different from the possible addition of a copy constructor
		virtual MiniGame* clone() = 0;
		//Setup method that initializa the wiimote pointer with one provided (amongst other actions)
		//This method is ALWAYS called AFTER the copy of the mini-games and should only perform actions independent of the level
		virtual void setup(WM* wiimote) = 0;
		//Update the mini-game and all its variables, all the computations should be done here so that draw only draws
		virtual void update() = 0;
		//Draw the mini-game depending of the provided size of the window
		virtual void draw(int size) = 0;
		//Should return true if the mini-game is solved
		virtual bool solved() = 0;
		//Used when mini-game entered in pause
		virtual void pause() = 0;
		//Used when a pause ends
		virtual void resume() = 0;
};