#include "SkillManager.h"
#include "Cube.h"

//--------------------------------------------------------------
SkillManager::SkillManager() { }

//--------------------------------------------------------------
SkillManager::SkillManager(list<Cube*>* listC) {
	_allCubes = listC;
	updated = false;
}

//--------------------------------------------------------------
SkillManager::~SkillManager() { }

//--------------------------------------------------------------
void SkillManager::addLevel(int id) {
	gameCompleted.push_front(id);

	list<unsigned int>::iterator it;
	list<unsigned int> visited;

	short same = 0;
	short different = 0;

	visited.push_back(gameCompleted.back());
	short lastPos = gameCompleted.size()-1;
	int i=gameCompleted.size();
	
	for(it = gameCompleted.begin(); it != gameCompleted.end(); it++) {
		if(*it != gameCompleted.front())
			break;
		same++;
	}

	if(same > 1) {
		different = 1;
	}
	else {
		it = gameCompleted.end();
		it--;
		while(it != gameCompleted.begin()) {
			it--;i--;
			for(list<unsigned int>::iterator vit = visited.begin(); vit != visited.end(); vit++) {
				if(*it == *vit) {
					lastPos = --i;
					visited.clear();
					if(it != gameCompleted.begin())
						it--;
					break;
				}
			}
			visited.push_back(*it);
		}
		different = lastPos+1;
	}

	if(different % 6 == 5) {
		skills.push_back(move);
	}
	else if(different % 6 == 3) {
		skills.push_back(move);
	}
	else if(different == 6) {
		skills.push_back(lock);
	}

	if(same % 4 == 0) {
		skills.push_back(lag);
	}

	if(same % 4 == 2) {
		skills.push_back(move);
	}

	for(list<Cube*>::iterator it = _allCubes->begin(); it != _allCubes->end(); it++)
		(*it)->skillManager->checkGap(gameCompleted.size());

}

//--------------------------------------------------------------
void SkillManager::removeLevel() {
	gameCompleted.pop_back();
}

//--------------------------------------------------------------
void SkillManager::changeSkill() {

	skills.push_back(skills.front());
	skills.pop_front();
}

//--------------------------------------------------------------
void SkillManager::activateSkill(Cube* sender) {
	if(!skills.empty()) {
		if(skills.front() == plus1) {
			sender->activeSkill(plus1);
		}
		else {
			for(list<Cube*>::iterator it = _allCubes->begin(); it != _allCubes->end(); it++)
				if((*it) !=sender)
					(*it)->activeSkill(skills.front());
		}
		skills.pop_front();
	}
}

//--------------------------------------------------------------
void SkillManager::checkGap(int number) {
	short counterSkills = 1;

	for(list<Skills>::iterator it = skills.begin(); it != skills.end(); it++)
		if(*it == plus1 || *it==minus2) counterSkills++;

	if(gameCompleted.size()+5*counterSkills <= number) {
		if(_allCubes->size() == 2)
			skills.push_back(minus2);
		else
			skills.push_back(plus1);
	}
}