#include "DemoCounter.h"

//--------------------------------------------------------------
DemoCounter::DemoCounter() {
	_started = false;
	_pausingTime = 0;
	_endingTime = 0;
}
		
//--------------------------------------------------------------
void DemoCounter::start(int time, int tops, int delay_before, int delay_after) {
	_timeDivision = (float)time / tops;
	_delayAfter = delay_after;
	_delayBefore = delay_before;
	_pausingTime = 0;
	_started = true;
	_currentTop = tops;
	_endingTime = _delayBefore + ofGetElapsedTimeMillis() + time + _delayAfter;
	
}

//--------------------------------------------------------------
void DemoCounter::pause() {
	_pausingTime = ofGetElapsedTimeMillis();
}
//--------------------------------------------------------------
void DemoCounter::resume() {
	if(_pausingTime)
		_endingTime += ofGetElapsedTimeMillis() - _pausingTime;
	_pausingTime = 0;
}


//--------------------------------------------------------------
unsigned int DemoCounter::getCurrentTop() {
	return _currentTop;
}

//--------------------------------------------------------------
bool DemoCounter::isActive() {
	return _started;
}

//--------------------------------------------------------------
void DemoCounter::stop() {
	_started = false;
}

//--------------------------------------------------------------
bool DemoCounter::newTop() {
	if(_started && _currentTop >= 0 && ofGetElapsedTimeMillis() > _endingTime - _delayAfter - _currentTop * _timeDivision) {
		_currentTop--;
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool DemoCounter::isFinished() {
	if(_started && _currentTop == 0) {
		stop();
		return true;
	}
	return false;
}

//--------------------------------------------------------------
void DemoCounter::accelerate() {
	_endingTime -= _timeDivision/2;
}