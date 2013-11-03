#include "MemWay.h"



//--------------------------------------------------------------
MemWay::MemWay() {}

//--------------------------------------------------------------
MemWay::MemWay(int level) {
	_sequenceLength = 4+level*2;
	
	restartMemWay:
	bool map[8][8] = {0};
	char x = (char)ofRandom(8);
	char y = (char)ofRandom(8);
	origin.x = x;
	origin.y = y;
	DIR available[4];
	char total_available;

	for(int i=0; i < _sequenceLength; i++) {
		total_available = 0;
		map[x][y] = 1;

		if(x > 0 && !map[x-1][y])
			available[total_available++] = N;
		if(x < 7 && !map[x+1][y])
			available[total_available++] = S;
		if(y > 0 && !map[x][y-1])
			available[total_available++] = W;
		if(y < 7 && !map[x][y+1])
			available[total_available++] = E;

		if(total_available == 0)
			goto restartMemWay;

		_sequence[i] = available[(int)ofRandom(total_available)];
		switch(_sequence[i]) {
			case N: x--; break;
			case S: x++; break;
			case W: y--; break;
			case E: y++; break;
		}
	}
}

//--------------------------------------------------------------
MemWay::~MemWay(){}

//--------------------------------------------------------------
MiniGame* MemWay::clone() {
	MemWay* toClone = new MemWay();
	toClone->_sequenceLength = _sequenceLength;
	toClone->origin = origin;
	for(int i=0 ; i<_sequenceLength; i++) {
		toClone->_sequence[i] = _sequence[i];
	}

	return toClone;
}

//--------------------------------------------------------------
void MemWay::setup(WM* wiimote) {
	_wiimote = wiimote;
	_background.loadImage(PATH+"back.png");
	_squareLeft.loadImage(PATH+"cubel.png");
	_squareRight.loadImage(PATH+"cuber.png");
	_squareUp.loadImage(PATH+"cubeu.png");
	_squareDown.loadImage(PATH+"cubed.png");
	_squareCenter.loadImage(PATH+"cubec.png");
	_redCross.init(PATH+"redcross.png");

	_demonstration.start(_sequenceLength*500, _sequenceLength+1, 1000);
	_progression = 0;
	_accerelationDate = 0;
}

//--------------------------------------------------------------
void MemWay::update() {
	if(_demonstration.isActive()) {
		if(_wiimote->Button.Two() && ofGetElapsedTimeMillis() > _accerelationDate) {
			_accerelationDate = ofGetElapsedTimeMillis() + 50;
			_demonstration.accelerate();
		}
		if(_demonstration.newTop()) {
			_progression++;
		}
		if(_demonstration.isFinished())
			_progression=0;
	}

	DIR dir = U;
	if(_wiimote->pressed(0, _wiimote->Button.Up())) dir = W;
	else if(_wiimote->pressed(1, _wiimote->Button.Down())) dir = E;
	else if(_wiimote->pressed(2, _wiimote->Button.Left())) dir = S;
	else if(_wiimote->pressed(3, _wiimote->Button.Right())) dir = N;

	if(dir != U) {
		if(_demonstration.isActive()) {  _demonstration.stop(); _progression=0; }
		if(_redCross.isActive()) { _redCross.stop(); _wiimote->SetRumble(false); }
		if(_sequence[_progression] == dir) {
			_progression++;
		}
		else if(!solved()) {
			_progression = 0;
			_redCross.start(500);
			_wiimote->SetRumble(true);
		}
	}

	if(_redCross.isFinished()) {
		_demonstration.start(_sequenceLength*500, _sequenceLength+1, 1000);
		_wiimote->SetRumble(false);
	}
}

//--------------------------------------------------------------
void MemWay::draw(int size) {
	ofEnableAlphaBlending();
	float const ratio = size/400.0f;
	ofPoint current(origin);
	_background.draw(0,0, 400*ratio, 400*ratio);
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);

	_squareCenter.draw(40*ratio + 40*ratio*origin.y, 40*ratio + 40*ratio*origin.x, 39*ratio, 39*ratio);
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);

	ofImage* toDraw;
	for(int i=0;i<_progression;i++) {
		switch(_sequence[i]) {
			case N: toDraw = &_squareUp; current.x--; break;
			case E: toDraw = &_squareRight; current.y++; break;
			case W: toDraw = &_squareLeft; current.y--; break;
			case S: toDraw = &_squareDown; current.x++; break;
		}

		if(_demonstration.isActive())
			ofSetColor(255,255,255,120);

		toDraw->draw(40*ratio + 40*ratio*current.y, 40*ratio + 40*ratio*current.x, 39*ratio, 39*ratio);
		ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);

		ofSetColor(255,255,255,255);
	}

	_redCross.draw(size);

	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
bool MemWay::solved() {
	return !_demonstration.isActive() && _progression >= _sequenceLength;
}

//--------------------------------------------------------------
void MemWay::pause() {
	_wiimote->SetRumble(0);
	_demonstration.pause();
}

//--------------------------------------------------------------
void MemWay::resume() {
	_demonstration.resume();
}