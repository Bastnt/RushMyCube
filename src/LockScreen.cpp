#include "LockScreen.h"


//--------------------------------------------------------------
LockScreen::LockScreen(){
	visitor.loadFont("fonts/visitor2.ttf",80, false);
	locker.loadImage(PATH+"lock.png");
	locker.setAnchorPercent(.5,.5);
	_endLockTime = 0;
	_remainingSeconds = 0;
	_remainingTime = "0:00";
	_active = false;
}

//--------------------------------------------------------------
LockScreen::~LockScreen() {}

//--------------------------------------------------------------
void LockScreen::draw(int size) {
	if(_active) {
		const long diff = (_endLockTime - ofGetElapsedTimeMillis())/1000;
		if(diff < _remainingSeconds) {
			_remainingSeconds = diff;
			stringstream s;
			if(_remainingSeconds/60 < 10)
				s << "0";
			s << _remainingSeconds/60 << ":";
			if(_remainingSeconds%60 < 10)
				s << "0";
			s << _remainingSeconds%60;

			_remainingTime = s.str();
		}
			
		const float ratio = size/400.0;

		ofPushStyle();
		ofSetColor(255);
		ofRect(0,0,size,size);
		ofEnableAlphaBlending();
		locker.draw(200.0*ratio, 150.0*ratio, 1.2*130.0*ratio, 1.2*160.0*ratio);
		ofDisableAlphaBlending();
		ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
		ofSetColor(0);
		visitor.drawString(_remainingTime, (size-198)*.5, (size-40)*.8);
		ofPopStyle();
	}
}

//--------------------------------------------------------------
bool LockScreen::isActive() {
	return _active;
}

//--------------------------------------------------------------
void LockScreen::start(int locktime) {
	_active = true;
	_remainingSeconds = INT_MAX;
	_endLockTime = ofGetElapsedTimeMillis() + locktime;
}

//--------------------------------------------------------------
void LockScreen::stop() {
	_active = false;
}

//--------------------------------------------------------------
bool LockScreen::isFinished() {
	if(_active) {
		if(ofGetElapsedTimeMillis() > _endLockTime) {
			_active = false;
			return true;
		}
	}
	return false;
}