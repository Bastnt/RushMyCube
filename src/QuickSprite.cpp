#include "QuickSprite.h"

//--------------------------------------------------------------
QuickSprite::QuickSprite() {
	_displayed = false;
}

//--------------------------------------------------------------
QuickSprite::~QuickSprite() {}

//--------------------------------------------------------------
void QuickSprite::init(string path, int backgroundColor) {
	_redCross.loadImage(path);
	_redCross.setAnchorPercent(.5, .5);
	_backgroundColor = backgroundColor;
}

//--------------------------------------------------------------
void QuickSprite::draw(int size) {
	if(_displayed) {
		ofPushStyle();
		ofEnableAlphaBlending();
		if(_backgroundColor != -1) {
			ofSetHexColor(_backgroundColor);
			ofRect(0,0,size,size);
		}
		_redCross.draw(size/2, size/2, 80*size/400, 80*size/400);
		ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void QuickSprite::start(int time_ms) {
	_displayed = true;
	_endingTime = ofGetElapsedTimeMillis() + time_ms;
}

//--------------------------------------------------------------
void QuickSprite::stop() {
	_displayed = false;
}

//--------------------------------------------------------------
bool QuickSprite::isActive() {
	return _displayed;
}

//--------------------------------------------------------------
bool QuickSprite::isFinished() {
	if(_displayed && ofGetElapsedTimeMillis() >= _endingTime) { stop(); return true; }
	return false;
}