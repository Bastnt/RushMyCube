#include "RotatorSprite.h"

//--------------------------------------------------------------
RotatorSprite::RotatorSprite() {}

//--------------------------------------------------------------
RotatorSprite::RotatorSprite(int color) {
	_color = color;
	displayed = false;
}

//--------------------------------------------------------------
RotatorSprite::~RotatorSprite() {}

//--------------------------------------------------------------
void RotatorSprite::draw(int size) {
	float borderDist = ((float) size) / 6.0;
	float radius = ((float) size) / 27.0;
	float between = 2 * radius;

	if(displayed) {
		ofPushStyle();
		ofFill();
		if(_color==0)
			ofSetColor(255,15,15);
		else if(_color==1)
			ofSetColor(15,255,15);
		else 
			ofSetColor(255,255,255);
		ofCircle(borderDist+radius+_displayX*(2*radius+between),
				borderDist+radius+_displayY*(2*radius+between),0,radius);
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void RotatorSprite::start(int time_ms,short x, short y) {
	_displayX = x;
	_displayY = y;
	displayed = true;	
	_endingTime = ofGetElapsedTimeMillis() + time_ms;
}

//--------------------------------------------------------------
void RotatorSprite::stop() {
	displayed = false;
}

//--------------------------------------------------------------
bool RotatorSprite::isActive() {
	return displayed;
}

//--------------------------------------------------------------
bool RotatorSprite::isFinished() {
	if(displayed && ofGetElapsedTimeMillis() >= _endingTime) { stop(); return true; }
	return false;
}