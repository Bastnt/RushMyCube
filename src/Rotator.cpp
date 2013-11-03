#include "Rotator.h"

//--------------------------------------------------------------
Rotator::Rotator() { }

//--------------------------------------------------------------
Rotator::Rotator(int level) {
	_level = level;
	_rotateTime = 0;
	_sequenceLength = (2*_level/3)+1;
	_sequence = new Position[_sequenceLength];
	for(int i=0 ; i<_sequenceLength; i++) {
		Position temp;
		do {
		temp.X= ofRandom(5);
		temp.Y= ofRandom(5);
		} while(temp.X==0 && temp.Y==0 || temp.X==0 && temp.Y==4 || temp.X==4 && temp.Y==0 || temp.X==4 && temp.Y==4);
		_sequence[i] = temp;
	}
}

//--------------------------------------------------------------
MiniGame* Rotator::clone() {
	Rotator* toClone = new Rotator();
	toClone->_level = _level;
	toClone->_sequenceLength = _sequenceLength;
	toClone->_sequence = new Position[_sequenceLength];
	for(int i=0 ; i<_sequenceLength; i++) {
		toClone->_sequence[i].X = _sequence[i].X;
		toClone->_sequence[i].Y = _sequence[i].Y;
	}

	return toClone;
}

//--------------------------------------------------------------
Rotator::~Rotator() {
	delete[] _sequence;
	delete _redCircle;
	delete _greenCircle;
	delete _whiteCircle;
	delete _demo;
}

//--------------------------------------------------------------
void Rotator::setup(WM* wiimote){	
	_wiimote = wiimote;
	_background.loadImage(PATH+"images/fond.png");
	_cursorImage.loadImage(PATH+"images/curseur2.png");
	_compass.loadImage(PATH+"images/compass.png");

	_redCircle = new RotatorSprite(0);
	_greenCircle = new RotatorSprite(1);
	_whiteCircle = new RotatorSprite(2);
	_demo = new DemoCounter();
	_rotation = 0;
	//cursor in the center
	_cursor.X = 2;
	_cursor.Y = 2;
	_progression = 0;
	_demoIndex = 0;
}

//--------------------------------------------------------------
void Rotator::update(){
	if(_redCircle->isFinished()) {
		_wiimote->SetRumble(false);
		launchDemo();
	}

	_greenCircle->isFinished();

	float elapsedTime = ofGetElapsedTimef();
	//-------------------------------------------------------------------------------------------------------------------
	// WIIMOTE CONTROLS (HORIZONTAL POSITION)
	//-------------------------------------------------------------------------------------------------------------------

	if(_wiimote->pressed(1, _wiimote->Button.One())) {
		if(!_demo->isActive())
			launchDemo();
		else {
			_demo->stop();
			_whiteCircle->stop();
		}
	}

	if(!_demo->isActive()) {
		if(_wiimote->pressed(0, _wiimote->Button.Two())) {
			if(_cursor.X==_sequence[_progression].X && _cursor.Y==_sequence[_progression].Y) {
				//good move
				_greenCircle->start(ANIMATION_LENGTH, _cursor.X, _cursor.Y);
				_progression++;
			}
			else {
				//bad move (loser!)
				_greenCircle->start(ANIMATION_LENGTH,_sequence[_progression].X, _sequence[_progression].Y);
				_redCircle->start(ANIMATION_LENGTH,_cursor.X, _cursor.Y);
				_progression=0;
				_wiimote->SetRumble(true);
			}
		}
	}
		
	if(!_demo->isActive()) {
		if(_wiimote->pressed(2, _wiimote->Button.Up()))
			leftMove();
		if(_wiimote->pressed(3, _wiimote->Button.Down()))
			rightMove();
		if(_wiimote->pressed(4, _wiimote->Button.Left()))
			upMove();
		if(_wiimote->pressed(5, _wiimote->Button.Right()))
			downMove();
	}

	if(ofGetElapsedTimeMillis() - _rotateTime > 8)
	{
		_rotateTime = ofGetElapsedTimeMillis();
		_rotation += 0.05 + 0.01*_level;
		if(_rotation>=360) _rotation=0.0;
	}

	//-------------------------------------------------------------------------------------------------------------------
	// BACKGROUND GRADIENT COLOR
	//-------------------------------------------------------------------------------------------------------------------
	gradientTransition(6.0-0.5*_level);
	
	//-------------------------------------------------------------------------------------------------------------------
	// DEMO (BEGINS WITH AN INTERVAL)
	//-------------------------------------------------------------------------------------------------------------------
	_demo->isFinished();
	_whiteCircle->isFinished();
	if(_demo->newTop()) {
		_whiteCircle->start(DEMO_ANIMATION_LENGTH, _sequence[_demoIndex].X, _sequence[_demoIndex].Y);
		_demoIndex++;
	}
}

//--------------------------------------------------------------
void Rotator::draw(int sizeBox) {
	float borderDist = ((float) sizeBox) / 6.0;
	float radius = ((float) sizeBox) / 27.0;
	float between = 2 * radius;

	ofPushMatrix();

	//-------------------------------------------------------------------------------------------------------------------
	// BACKGROUND (+GRADIENT COLOR)
	//-------------------------------------------------------------------------------------------------------------------
	
	ofPushStyle();
	ofSetColor(_backgroundColor);
	_background.draw(0,0,0,sizeBox,sizeBox);
	ofPopStyle();

	ofPushMatrix();
	ofTranslate(8*sizeBox/9, sizeBox/9,0);
	ofRotateZ(_rotation);

	//compass
	ofEnableAlphaBlending();
	_compass.setAnchorPercent(0.5,0.5);
	_compass.draw(0,0,0,3*radius,3*radius);
	ofDisableAlphaBlending();
	ofPopMatrix();

	ofTranslate(sizeBox/2, sizeBox/2,0);
	ofRotateZ(_rotation);
	ofTranslate(-sizeBox/2, -sizeBox/2,0);

	//-------------------------------------------------------------------------------------------------------------------
	// CIRCLES IN THE BACKGROUND
	//-------------------------------------------------------------------------------------------------------------------
	ofPushStyle();

	//first line
	float xDrawing = borderDist+3*radius+between;
	float yDrawing = borderDist+radius;
	for(int i=1 ; i<4 ; i++) {
		ofSetColor(255,255,255);
			ofFill();
			ofCircle(xDrawing,yDrawing,0,radius+1);
			ofSetColor(0,0,0);
			ofCircle(xDrawing,yDrawing,0,radius);
		xDrawing+= between+2*radius;
	}
	//lines between
	xDrawing = borderDist+ radius;
	yDrawing += between+2*radius;
	for(int j=1; j<4 ; j++) {
		for(int i=0; i<5 ;i++) {
			ofSetColor(255,255,255);
			ofFill();
			ofCircle(xDrawing,yDrawing,0,radius+1);
			ofSetColor(0,0,0);
			ofCircle(xDrawing,yDrawing,0,radius);
			xDrawing+= between+2*radius;
		}
		xDrawing = borderDist+radius;
		yDrawing += between+2*radius;
	}
	//last line
	xDrawing = borderDist+3*radius+between;
	for(int i=1 ; i<4 ; i++) {
		ofSetColor(255,255,255);
			ofFill();
			ofCircle(xDrawing,yDrawing,0,radius+1);
			ofSetColor(0,0,0);
			ofCircle(xDrawing,yDrawing,0,radius);
		xDrawing+= between+2*radius;
	}
	ofPopStyle();

	//-------------------------------------------------------------------------------------------------------------------
	// DEMO MODE : CIRCLE IN THE FOREGROUND
	//-------------------------------------------------------------------------------------------------------------------
	_whiteCircle->draw(sizeBox);

	//-------------------------------------------------------------------------------------------------------------------
	// ANIMATION (SUCCESS AND FAIL) : CIRCLE IN THE FOREGROUND
	//-------------------------------------------------------------------------------------------------------------------
	_greenCircle->draw(sizeBox);
	_redCircle->draw(sizeBox);

	//-------------------------------------------------------------------------------------------------------------------
	// CURSOR
	//-------------------------------------------------------------------------------------------------------------------
	if(!(_whiteCircle->isActive() || _demo->isActive())) {
		//cursor
		ofPushStyle();
		ofSetColor(255,255,255);
		ofEnableAlphaBlending();
		_cursorImage.setAnchorPercent(0.5,0.5);
		_cursorImage.draw(borderDist+radius+_cursor.X*(2*radius+between),borderDist+radius+_cursor.Y*(2*radius+between),0,3.5*radius,3.5*radius);
		ofDisableAlphaBlending();
		ofPopStyle();
	}

	ofPopMatrix();	
}

//--------------------------------------------------------------
bool Rotator::solved() { return _progression >= _sequenceLength; }

//--------------------------------------------------------------
void Rotator::pause() {
	_pauseStartingTime = ofGetElapsedTimef();
	_wiimote->SetRumble(0);
}

//--------------------------------------------------------------
void Rotator::resume() {
	float pauseDuration= ofGetElapsedTimef() - _pauseStartingTime;
	if(_progression != 0)
		_greenCircle->start(ANIMATION_LENGTH,_sequence[_progression].X,_sequence[_progression].Y);
}

//--------------------------------------------------------------
void Rotator::leftMove() {
	if((_cursor.Y > 0 && _cursor.Y < 4 && _cursor.X > 0) || _cursor.X > 1) 
				{_cursor.X -= 1;}
}

//--------------------------------------------------------------
void Rotator::rightMove() {
	if((_cursor.Y > 0 && _cursor.Y < 4 && _cursor.X < 4) || _cursor.X < 3) 
				{_cursor.X += 1;}
}

//--------------------------------------------------------------
void Rotator::upMove() {
	if((_cursor.X > 0 && _cursor.X < 4 && _cursor.Y < 4) || _cursor.Y < 3) 
				{_cursor.Y += 1;}
}

//--------------------------------------------------------------
void Rotator::downMove() {
	if((_cursor.X > 0 && _cursor.X < 4 && _cursor.Y > 0) || _cursor.Y > 1) 
				{_cursor.Y -= 1;}
}

//--------------------------------------------------------------
void Rotator::gradientTransition(float animationLength) {
	if(animationLength <= 0)
		animationLength=0.5;
	static float animationColorStarting=0;
	static short colorToChange=0;
	//full blue is ugly so the value is not 255 for blue.
	float progression = (ofGetElapsedTimef() - animationColorStarting) / animationLength;
	
	if(progression <= 1.0) {
		switch(colorToChange) {
		case 0: _backgroundColor = colorTransition(ofColor(240,40,40),ofColor(240,240,40),progression); break;
		case 1: _backgroundColor = colorTransition(ofColor(240,240,40),ofColor(40,240,40),progression); break;
		case 2: _backgroundColor = colorTransition(ofColor(40,240,40),ofColor(40,240,240),progression); break;
		case 3: _backgroundColor = colorTransition(ofColor(40,240,240),ofColor(80,40,210),progression); break;
		case 4: _backgroundColor = colorTransition(ofColor(80,40,210),ofColor(240,40,240),progression); break;
		case 5: _backgroundColor = colorTransition(ofColor(240,40,240),ofColor(240,40,40),progression); break;
		}
	}
	else {
		animationColorStarting = ofGetElapsedTimef();
		colorToChange++;
		if(colorToChange>5) colorToChange=0;
	}
}

//--------------------------------------------------------------
ofColor Rotator::colorTransition(const ofColor& c1,const ofColor& c2, float progression) {
	ofColor ans;
	ans.r = c1.r + progression*(c2.r - c1.r);
	ans.g = c1.g + progression*(c2.g - c1.g);
	ans.b = c1.b + progression*(c2.b - c1.b);
	return ans;
}

//--------------------------------------------------------------
void Rotator::launchDemo() {
	_demoIndex = 0;
	_demo->start((DEMO_ANIMATION_LENGTH+DEMO_ANIMATION_INTERVAL)*_sequenceLength,_sequenceLength,DEMO_ANIMATION_INTERVAL);
}