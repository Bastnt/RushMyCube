#include "RocketShake.h"


//--------------------------------------------------------------
RocketShake::RocketShake() { }

//--------------------------------------------------------------
RocketShake::RocketShake(int level) {
	_level = level;
	_positionPlanet1 = ofVec2f(ofRandom(1), ofRandom(1));
	_positionPlanet2 = ofVec2f(ofRandom(1), ofRandom(1));
}

//--------------------------------------------------------------
RocketShake::~RocketShake() {}

//--------------------------------------------------------------
MiniGame* RocketShake::clone() {
	RocketShake* toClone = new RocketShake();
	toClone->_level = _level;
	toClone->_positionPlanet1 = _positionPlanet1;
	toClone->_positionPlanet2 = _positionPlanet2;

	return toClone;
}

//--------------------------------------------------------------
void RocketShake::setup(WM* wiimote) {
	_wiimote = wiimote;
	_rocketHeight = 0;
	_date = 0;
	_speed = 0;

	_background.loadImage(PATH+"back.png");
	_moon.loadImage(PATH+"moon.png");
	_planet1.loadImage(PATH+"planet1.png");
	_planet2.loadImage(PATH+"planet2.png");
	_rocket.loadImage(PATH+"rocket.png");
	_fire.loadImage(PATH+"fire.png");
	_fire.setAnchorPercent(.5,0);
	_moon.setAnchorPercent(.5,1);
	_rocket.setAnchorPercent(.5,1);
	_planet1.setAnchorPercent(.5,.5);
	_planet2.setAnchorPercent(.5,.5);

	_wiimote->enableAccCapture();
}

//--------------------------------------------------------------
void RocketShake::update() {
	if(ofGetElapsedTimeMillis() > _date) {
		_date = ofGetElapsedTimeMillis() + 15;
		if(_speed > -2.5) _speed-=.03;
		int accel = (_wiimote->Acceleration.X() > 0 ? _wiimote->Acceleration.X() : 0);
		accel += (_wiimote->Acceleration.Y() > 0 ? _wiimote->Acceleration.Y() : 0);
		if(_speed < 2.5) _speed+=1.3*accel/_level/20.0;

		_rocketHeight+=_speed/600.0;
		if(_rocketHeight < 0) {_rocketHeight = 0; _speed = 0;}
		else if(_rocketHeight > 1) _rocketHeight = 1;
	}
}

//--------------------------------------------------------------
void RocketShake::draw(int size) {
	ofEnableAlphaBlending();
	_background.draw(0,0,size,size);
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
	_planet1.draw(_positionPlanet1.x*size, _positionPlanet1.y*size, .15*size, .15*size);
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
	_planet2.draw(_positionPlanet2.x*size, _positionPlanet2.y*size, .1*size, .1*size);
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
	_moon.draw(size/2, size, .5*size, .12*size);
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
	_fire.draw(size/2, (size-.10*size)*(1-_rocketHeight)-.10*size, (_speed < 0 ? 0 : _speed)/2.5*.05*size, (_speed < 0 ? 0 : _speed)/2.5*.17*size);
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
	_rocket.draw(size/2, (size-.10*size)*(1-_rocketHeight), .12*size, .32*size);
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
bool RocketShake::solved() {
	return (_rocketHeight == 1);
}

//--------------------------------------------------------------
void RocketShake::pause() {
	_wiimote->disableAccCapture();
}

//--------------------------------------------------------------
void RocketShake::resume() {
	_wiimote->enableAccCapture();
}
