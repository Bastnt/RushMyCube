#pragma once
#include <list>
using namespace std;

class Cube;

class SkillManager {
public:
	//Enumeration of all the skills, there are explained in the report
	enum Skills { lag, move, lock, plus1, minus2 };
	//Remember the list of completed games
	list<unsigned int> gameCompleted;
	//The current possessed skills
	list<Skills> skills;
	//True if the SkillManager is updated
	bool updated;
	//Reference toward the other cubes
	list<Cube*>* _allCubes;

	//Constructors and destructor
	//---------------------------------------------------------------------------------
	//Default constructor : do nothing
	SkillManager();
	//Create a SkillManager using the reference to initializa _allCubes correctly
	SkillManager(list<Cube*>* listC);
	//Destructor
	~SkillManager();
	
	//Subfunctions
	//---------------------------------------------------------------------------------
	//Add a level to the gameCompleted list and grant a skill if the conditions are met
	void addLevel(int id);
	//Remove a level from the gameCompleted list
	void removeLevel();
	//Change the order of the skills list
	void changeSkill();
	//Check the gap between the number of solved games and the number of solved games of the other players
	void checkGap(int number);
	//Use the first skill of the skills list
	void activateSkill(Cube* sender);};