#include "Cube.h"


//--------------------------------------------------------------
Cube::Cube() { }

//--------------------------------------------------------------
Cube::Cube(unsigned short startingLevel, unsigned short numberofLevel) {
	_miniGames = new GameManager<MiniGame>*[NB_FACES];
	list<char> games;
	for(int i=0;i<NB_FACES;i++)
		games.push_front(i);

	for(int i=0;i<NB_FACES;i++) {
			int r = ofRandom(games.size());
			list<char>::iterator it = games.begin();
			advance(it, r);
			switch(*it) {
				case 0: _miniGames[i] = (GameManager<MiniGame>*) new GameManager<Rotator>(startingLevel, numberofLevel);  break;
				case 1: _miniGames[i] = (GameManager<MiniGame>*) new GameManager<MemWay>(startingLevel, numberofLevel); break;
				case 2: _miniGames[i] = (GameManager<MiniGame>*) new GameManager<KoloruSuicchi>(startingLevel, numberofLevel); break;
				case 3: _miniGames[i] = (GameManager<MiniGame>*) new GameManager<RocketShake>(startingLevel, numberofLevel); break;
				case 4: _miniGames[i] = (GameManager<MiniGame>*) new GameManager<MathRush>(startingLevel, numberofLevel); break;
				case 5: _miniGames[i] = (GameManager<MiniGame>*) new GameManager<PuzzleLeagueLike>(startingLevel, numberofLevel); break;
			}
			games.remove(*it);

	}
}
		
//--------------------------------------------------------------
Cube* Cube::clone() {
	Cube* toClone = new Cube();
	toClone->_miniGames = new GameManager<MiniGame>*[NB_FACES];
	for(int i=0;i<NB_FACES;i++) {
		toClone->_miniGames[i] = _miniGames[i]->clone();
	}

	return toClone;
}
		
//--------------------------------------------------------------
Cube::~Cube() {
	delete[] _miniGames;
}

//--------------------------------------------------------------
void Cube::setup(WM* wiimote, list<Cube*>* opponents) {
	_wiimote = wiimote;
	skillManager = new SkillManager(opponents);

	skillManager->skills.push_back(SkillManager::Skills::lag);

	for(int i=0 ; i<NB_FACES ; i++) {
		_miniGames[i]->setup(_wiimote, skillManager);
		_miniGames[i]->pause();
	}
	
	rotationDir = ofVec3f(0,0,0);
	rotationInit = ofVec3f(0,0,0);
	_rotation=0;

	//the front one is first active
	_active = 0;
	_dateLag = 0;
	_miniGames[0]->resume();
	
}

//--------------------------------------------------------------
void Cube::update() {
	if(_wiimote->pressed(10, _wiimote->Button.Plus())) {
		skillManager->activateSkill(this);
	}

	if(_wiimote->pressed(11, _wiimote->Button.Minus())) {
			skillManager->changeSkill();
	}

	if(_rotation==0) {
		if( _wiimote->Button.B() && _wiimote->Button.Up())
			Move(1);
		if(_wiimote->Button.B() && _wiimote->Button.Down()) 
			Move(3);
		if(_wiimote->Button.B() && _wiimote->Button.Right()) 
			Move(2);
		if(_wiimote->Button.B() && _wiimote->Button.Left()) 
			Move(4);
	}

	if(_rotation==0)
		_miniGames[_active]->update();

	if(rotate.newTop()){
		switch(_rotation) {
            case 4: rotationDir.x -=3 ; break;
			case 3: rotationDir.y -= 3; break;
            case 1: rotationDir.y += 3; break; //Right case
            case 2: rotationDir.x += 3; break; //Down case
            default: break;
        }
	}

	if(rotate.isFinished()) {
		switch(_rotation) {
            case 4: rotationInit.x = rotationDir.x = ((int) rotationInit.x-90)%360; break;
			case 3: rotationInit.y = rotationDir.y = ((int) rotationInit.y-90)%360; break;
            case 1: rotationInit.y = rotationDir.y = ((int) rotationInit.y+90)%360; break; //Right case
            case 2: rotationInit.x = rotationDir.x = ((int) rotationInit.x+90)%360; break; //Down case
            default: break;
        }
		_rotation=0;
		_miniGames[_active]->resume();
	}
}

//--------------------------------------------------------------
bool Cube::solved() {
	for(int i=0;i<NB_FACES;i++)
		if(!_miniGames[i]->solved())
			return false;
	return true;
}

//--------------------------------------------------------------
void Cube::draw(int size) {
	if(!lagger.isActive() || ofGetElapsedTimeMillis() < _dateLag) {
		lagger.isFinished();

		ofPushMatrix();

		const int halfSize = size/2;

		ofRotateX(rotationDir.x);
		ofRotateY(180+rotationDir.y);

		ofRotateZ(180);

		for(int i=0;i<NB_FACES;i++) {
			ofTranslate(-halfSize, -halfSize, -halfSize);
			if(_active == i || rotate.isActive() && _prevActive == i)
			_miniGames[i]->draw(size);
			ofTranslate(halfSize, halfSize, halfSize);
			if(i == 3)
				ofRotateX(90);
			else if(i==4)
				ofRotateX(180);
			else
				ofRotateY(90);
		}

		ofPopMatrix();
	}
	else {
		if(lagger.newTop())
			_dateLag = ofGetElapsedTimeMillis() + 100;
	}
}

//--------------------------------------------------------------
void Cube::drawAllFaces(int size) {
		ofPushMatrix();
		const int halfSize = size/2;
		ofRotateX(rotationDir.x);
		ofRotateY(180+rotationDir.y);

		ofRotateZ(180);

		for(int i=0;i<NB_FACES;i++) {
			ofTranslate(-halfSize, -halfSize, -halfSize);
			_miniGames[i]->draw(size);
			ofTranslate(halfSize, halfSize, halfSize);
			if(i == 3)
				ofRotateX(90);
			else if(i==4)
				ofRotateX(180);
			else
				ofRotateY(90);
		}

		ofPopMatrix();
}

//--------------------------------------------------------------
void Cube::activeSkill(SkillManager::Skills s) {
	switch(s) {
	case SkillManager::Skills::lag: lagger.start(6000, 10); break;
	case SkillManager::Skills::move: if(_rotation == 0) Move(ofRandom(0, 5)); break;
	case SkillManager::Skills::lock: _miniGames[_active]->lock(8000); break;
	case SkillManager::Skills::plus1: _miniGames[_active]->nextLevel(); break;
	case SkillManager::Skills::minus2: _miniGames[_active]->previousLevel();_miniGames[_active]->previousLevel(); break;
	}
}

//--------------------------------------------------------------
void Cube::Move(int dir) {
	_rotation = dir;
	_miniGames[_active]->pause();
	_prevActive = _active;
	_active = nextActive(dir);
	rotate.start(500, 30);
	rotationDir = rotationInit;
}

//--------------------------------------------------------------
int Cube::nextActive(int rotation) {
	ofVec3f rotationInit2 = ofVec3f(rotationInit);	
	switch(rotation) {
		case 4: rotationInit2.x = ((int) rotationInit.x-90)%360; break;
		case 3: rotationInit2.y = ((int) rotationInit.y-90)%360; break;
		case 1: rotationInit2.y = ((int) rotationInit.y+90)%360; break; //Right case
		case 2: rotationInit2.x = ((int) rotationInit.x+90)%360; break; //Down case
		default: break;
	}

	if(rotationInit2.x < 0)
		rotationInit2 += 360;
	if(rotationInit2.y < 0)
		rotationInit2.y += 360;

	if(((int) rotationInit2.x % 360)==0) {
		switch((int) rotationInit2.y % 360) {
		case 0: return 0;
		case 90: return 1;
		case 180: return 2;
		case 270: return 3;
		default:break;
		}
	}
	else if(((int) rotationInit2.x % 360)==180) {
		switch((int) rotationInit2.y % 360) {
		case 0: return 2;
		case 90: return 3;
		case 180: return 0;
		case 270: return 1;
		default:break;
		}
	}
	else if(((int) rotationInit2.x % 360)==90 ) {
		return 5;
	}
	else if(((int) rotationInit2.x % 360)==270 ) {
		return 4;
	}
	//should never happened
	return _active;
	
}