#pragma once
#include "MiniGame.h"
#include "GameManager.h"

using namespace std;

//The number of faces of the cube (we first hesitated with only 4)
#define NB_FACES 6

class Cube {
	private:
		//Cube attributes
		//---------------------------------------------------------------------------------
		GameManager<MiniGame>** _miniGames;
		//Controller
		WM* _wiimote;
		//The number of the active mini-game
		char _active;
		//The previous active game
		char _prevActive;
		
		//Graphics
		//---------------------------------------------------------------------------------
		//The DemoCounter object of the rotation
		DemoCounter rotate;
		//The DemoCounter object of the lag power/skill
		DemoCounter lagger;
		//Remember the actual position of rotation of the cube in 3D
		ofVec3f rotationDir;
		//Remember the previous position of rotation of the cube in 3D
		ofVec3f rotationInit;
		//The direction of the rotation
		int _rotation;
		//Remember the start of the lag power/skill
		unsigned long _dateLag;
		
		//Subfunctions
		//---------------------------------------------------------------------------------
		//Rotate the cube in a direction
		void Move(int dir);
		//Return value of the active if the provided rotation occurs
		int nextActive(int rotation);
	public:
		//Manage the skills of the player of the cube 
		SkillManager* skillManager;
	
		//Constructors and destructor
		//---------------------------------------------------------------------------------
		//Default constructor : do nothing
		Cube();
		//Create a cube with the provided starting level and number of level per mini-game
		Cube(unsigned short startingLevel, unsigned short numberofLevel);
		//Destructor
		virtual ~Cube();
		
		//Subfunctions
		//---------------------------------------------------------------------------------
		//Setup the cube using the list of opponents and controller pointer
		void setup(WM* wiimote, list<Cube*>* opponents);
		//Create a clone of the cube
		Cube* clone();
		//U^date the variables of the cube
		void update();
		//The cube depending of the provided window size
		void draw(int size);
		//Draw all the faces and not only the active one
		void drawAllFaces(int size);
		//True if the cube is totally solved
		bool solved();
		//Use a skill
		void activeSkill(SkillManager::Skills s);
};