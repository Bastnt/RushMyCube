#pragma once
#include "ofMain.h"
#include "Cube.h"
#include "WM.h"
#include "DemoCounter.h"
#include "StopWatch.h"
#include <cmath>

//The default sizes of the window
#define DEFAULT_HEIGHT 700
#define DEFAULT_WIDTH 1010

//The maximum number of players allowed
#define MAX_PLAYERS 4


class RushMyCube : public ofBaseApp {
	private:
		//Graphics
		//---------------------------------------------------------------------------------
		//The controllers
		WM* remote[MAX_PLAYERS];
		//The ofCamera object used to see what is going on
		ofCamera* cam[MAX_PLAYERS];
		//Display informations
		int drawSize, camWidth;
		//The list of cubes of mini-games
		list<Cube*> cubes;
		//The player number of the winner
		int winner;

		//Graphics
		//---------------------------------------------------------------------------------
		//The ofImage of the lag power icon
		ofImage lag;
		//The ofImage of the lock power icon
		ofImage lock;
		//The ofImage of the plus1 power icon
		ofImage plus1;
		//The ofImage of the minus2 power icon
		ofImage minus2;
		//The ofImage of the move/switch power icon
		ofImage move;
		//The stopwatch displayed at the bottom
		StopWatch sw;
		//The DemoCounter object for the final animation
		DemoCounter _finish;
		//The police used to displayed textual inforamations
		ofTrueTypeFont winFont;
		
		

	public:
		//Inheritance
		//---------------------------------------------------------------------------------
		void setup();
		void update();
		void draw();
		//Events
		//---------------------------------------------------------------------------------
		void keyPressed (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
};
