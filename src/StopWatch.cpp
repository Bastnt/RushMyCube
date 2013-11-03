#include "StopWatch.h"

//--------------------------------------------------------------
StopWatch::StopWatch() {
	_visitor.loadFont("fonts/visitor2.ttf", 80, false);
	_startingTime = 0;
	_pausingTime = 0;
	_elapsedSeconds = 0;
	_actualTime = "00:00";
	_started = false;
}

//--------------------------------------------------------------
StopWatch::~StopWatch() {}

//--------------------------------------------------------------
void StopWatch::draw(int size, float x, float y) {
	if(_started) {
		_visitor.drawString(_actualTime, x - _anchor50.x, y + _anchor50.y);
	}
}

//--------------------------------------------------------------
void StopWatch::update() {
	if(_started && !_pausingTime) {
		const unsigned int diff = (ofGetElapsedTimeMillis() - _startingTime)/1000;
		if(diff > _elapsedSeconds) {
			_elapsedSeconds = diff;
			stringstream s;
			if(_elapsedSeconds/60 < 10)
				s << "0";
			s << _elapsedSeconds/60 << ":";
			if(_elapsedSeconds%60 < 10)
				s << "0";
			s << _elapsedSeconds%60;

			_actualTime = s.str();
		}
	}
}

//--------------------------------------------------------------
void StopWatch::start() {
	_startingTime = ofGetElapsedTimeMillis();
	_started = true;
	_pausingTime = 0;

	ofRectangle dim = _visitor.getStringBoundingBox(_actualTime, 0, 0);
	_anchor50.x = dim.getWidth()/2;
	_anchor50.y = dim.getHeight()/2;
}

//--------------------------------------------------------------
void StopWatch::pause() {
	_pausingTime = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
void StopWatch::resume() {
	if(_pausingTime)
		_startingTime += ofGetElapsedTimeMillis() - _pausingTime;
	_pausingTime = 0;
}

//--------------------------------------------------------------
void StopWatch::stop() {
	_started = false;
}