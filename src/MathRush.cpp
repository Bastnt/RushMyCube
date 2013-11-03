#include "MathRush.h"

//--------------------------------------------------------------
MathRush::MathRush() { }

//--------------------------------------------------------------
MathRush::MathRush(int level) {
	_sequenceLength = 2+level*2;
	const short lvl = level-1;
	
	restartMathRush:
	bool map[8][8] = {0};
	for(int i=0;i<8;i++)
		fill_n(_matrix[i], 8, 0);

	char x = (char)ofRandom(8);
	char y = (char)ofRandom(8);
	_origin.x = x;
	_origin.y = y;

	DIR available[4];
	char total_available;
	int init = ofRandom(5-5*lvl*lvl, 5*lvl*lvl);
	int current = init;
	_next = ofRandom(level*(level/5+1), 4*level*(level/5+1));
	if(ofRandom(1) < .1*lvl) _next = -_next;

	for(int i=0; i < _sequenceLength; i++) {
		total_available = 0;
		map[x][y] = 1;
		_matrix[x][y] = current;

		if(x > 0 && !map[x-1][y]) {
			available[total_available++] = N;
			if(!_matrix[x-1][y]) {
				if((_next > 0 ? _next : -_next) > 16 && ofRandom(3) > 2)
					_matrix[x-1][y] = current+ofRandom(3/level, 2+4/level)+_next;
				else
					_matrix[x-1][y] = current + _next + (_next > 0 ? 10 : -10);
			}
		}
		if(x < 7 && !map[x+1][y]) {
			available[total_available++] = S;
			if(!_matrix[x+1][y])
				if((_next > 0 ? _next : -_next) > 16 && ofRandom(3) > 2)
					_matrix[x+1][y] = current+ofRandom(3/level, 2+4/level)+_next;
				else
					_matrix[x+1][y] = current + _next + (_next > 0 ? 10 : -10);
		}
		if(y > 0 && !map[x][y-1]) {
			available[total_available++] = W;
			if(!_matrix[x][y-1])
				if((_next > 0 ? _next : -_next) > 16 && ofRandom(3) > 2)
					_matrix[x][y-1] = current+ofRandom(3/level, 2+4/level)+_next;
				else
					_matrix[x][y-1] = current + _next + (_next > 0 ? 10 : -10);
		}
		if(y < 7 && !map[x][y+1]) {
			available[total_available++] = E;
			if(!_matrix[x][y+1])
				if((_next > 0 ? _next : -_next) > 16 && ofRandom(3) > 2)
					_matrix[x][y+1] = current+ofRandom(3/level, 2+4/level)+_next;
				else
					_matrix[x][y+1] = current + _next + (_next > 0 ? 10 : -10);
		}

		if(total_available == 0)
			goto restartMathRush;

		_sequence[i] = available[(int)ofRandom(total_available)];

		current+=_next;

		switch(_sequence[i]) {
			case N: x--; break;
			case S: x++; break;
			case W: y--; break;
			case E: y++; break;
		}
	}

	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
			if(!_matrix[i][j] && !map[i][j]) _matrix[i][j] = init + _next*ofRandom(0,_sequenceLength); 
}

//--------------------------------------------------------------
MathRush::~MathRush() {}

//--------------------------------------------------------------
MiniGame* MathRush::clone() {
	MathRush* toClone = new MathRush();
	toClone->_origin = _origin;
	toClone->_next = _next;
	toClone->_sequenceLength = _sequenceLength;
	int res[8][8];
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
			toClone->_matrix[i][j] = _matrix[i][j];
	return toClone;
}

//--------------------------------------------------------------
void MathRush::setup(WM* wiimote) {
	_wiimote = wiimote;

	_background.loadImage(PATH+"back.png");
	_redCross.init(PATH+"redcross.png");
	_visitor.loadFont("fonts/visitor2.ttf",28, false);

	_progression = 0;
	_currentTotal = _matrix[(int)_origin.x][(int)_origin.y];
	_current = ofVec2f(_origin);
}

//--------------------------------------------------------------
void MathRush::update() {
	DIR dir = U;
	if(_wiimote->pressed(0, _wiimote->Button.Up())) dir = W;
	else if(_wiimote->pressed(1, _wiimote->Button.Down())) dir = E;
	else if(_wiimote->pressed(2, _wiimote->Button.Left())) dir = S;
	else if(_wiimote->pressed(3, _wiimote->Button.Right())) dir = N;

	if(dir != U) {
		if(_redCross.isActive()) { _redCross.stop(); _wiimote->SetRumble(false); }
		switch(dir) {
			case N: _current.x--; break;
			case E: _current.y++; break;
			case W: _current.y--; break;
			case S: _current.x++; break;

		}
		if(_matrix[(int)_current.x][(int)_current.y] == _currentTotal + _next) {
			_currentTotal += _next;
			_progression++;
		}
		else if(!solved()) {
			_progression = 0;
			_current = ofVec2f(_origin);
			_currentTotal = _matrix[(int)_origin.x][(int)_origin.y];
			_redCross.start(500);
			_wiimote->SetRumble(true);
		}
	}

	if(_redCross.isFinished()) {
		_wiimote->SetRumble(false);
	}
}

//--------------------------------------------------------------
void MathRush::draw(int size) {
	const int boxSize = size*.0935;
	ofEnableAlphaBlending();
	_background.draw(0,0,size,size);
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);

	stringstream s;
	if(_next > 0)
		s << "+";
	s<< _next;
	ofPushMatrix();
	ofPushStyle();
	ofRectangle r = _visitor.getStringBoundingBox(s.str(), 0, 0);
	float scal = size *.0038;
	ofScale(scal,scal);
	ofSetColor(255, 255, 255);
	_visitor.drawString(s.str(), (size - scal*r.getWidth() )/2/scal, r.getHeight()+scal);
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
	ofPopStyle();
	ofPopMatrix();

	ofPushMatrix();
	ofPushStyle();
	ofTranslate(boxSize, boxSize);
	
	for(int i=0;i<8;i++) {
		ofPushMatrix();
		for(int j=0;j<8;j++) {
			if(i == _current.x && j==_current.y)
				ofSetColor(100, 100, 100);
			else
				ofSetColor(0, 0, 0);

			ofRect(0,0,boxSize,boxSize);
			ofSetColor(30, 210, 30);
			stringstream s;
			s << _matrix[i][j];
			r = _visitor.getStringBoundingBox(s.str(), 0, 0);
			ofPushMatrix();
			float scal = 1;
			if(r.getWidth() > boxSize) {
				scal = (float)boxSize / r.getWidth();
				ofScale(scal,scal);
			}
			_visitor.drawString(s.str(), (boxSize - scal*r.getWidth()) /2 , (boxSize + scal*r.getHeight()) /2);
			ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
			ofPopMatrix();
			ofTranslate(boxSize + boxSize*.1, 0);
		}
		ofPopMatrix();
		ofTranslate(0, boxSize + boxSize*.1);
	}

	ofPopStyle();
	ofPopMatrix();

	_redCross.draw(size);

	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
bool MathRush::solved() {
	return _progression >= _sequenceLength-1;
}

//--------------------------------------------------------------
void MathRush::pause() {
	_wiimote->SetRumble(0);
}

//--------------------------------------------------------------
void MathRush::resume() {
	
}