#include "KoloruSuicchi.h"

//When adding colors : do not forget to update NUMBER_COLORS in KoloruSuicchi.h as well.
const ofColor KoloruSuicchi::COLORS[] = {ofColor(0,128,0), ofColor(250,128,114), ofColor(255,215,0), ofColor(0,0,128), ofColor(224,0,0), ofColor(239,255,0), ofColor(0,255,0), 
	ofColor(0,239,239), ofColor(128,128,128), ofColor(128,0,128), ofColor(245,245,220), ofColor(0,0,0), ofColor(192,255,62), ofColor(255,127,0), ofColor(255,0,147), ofColor(61,89,171),
	ofColor(255,255,255)};
const string KoloruSuicchi::NAMES[] = {"green", "salmon", "gold", "navy", "red", "yellow", "lime", "cyan", "gray", "purple", "beige", "black", "olive", "orange","pink", "cobalt","white"};

//--------------------------------------------------------------
KoloruSuicchi::KoloruSuicchi() { }

//--------------------------------------------------------------
KoloruSuicchi::KoloruSuicchi(int level) {
	_level = level;
	levelGenerator();
}

//--------------------------------------------------------------
void KoloruSuicchi::levelGenerator() {
	_matrixSize = _level/3 +4;
	if(_matrixSize > 8) 
		_matrixSize = 8;

	int rand = ofRandom(2);
	rand==1 ? (_matchBackground=false) : (_matchBackground=true);

	_availableColors = _level + 2.0;
	if(_availableColors > NUMBER_COLORS)
		_availableColors = NUMBER_COLORS;

	_matrix = new SquareColor*[_matrixSize];
	for(int i=0; i<_matrixSize ; i++) {
		_matrix[i] = new SquareColor[_matrixSize];
		for(int j=0; j<_matrixSize ; j++) {
			SquareColor tmp;
			int position1 = (int) ofRandom(_availableColors);
			int position2;
			do {
				position2 = (int) ofRandom(_availableColors);
			} while(position1 == position2);
			int position3 = (int) ofRandom(_availableColors);
			if(_matchBackground) {
				tmp.background = ofColor(COLORS[position2]);
				tmp.color = ofColor(COLORS[position3]);
				tmp.name = string(NAMES[position1]);
			}
			else {
				tmp.background = ofColor(COLORS[position3]);
				tmp.color = ofColor(COLORS[position2]);
				tmp.name = string(NAMES[position1]);
			}
			_matrix[i][j] = tmp;
		}
	}
	_answerLine = (int) ofRandom(_matrixSize);
	_answerColumn = (int) ofRandom(_matrixSize);
	int goodOne = (int) ofRandom(_availableColors);
	if(_matchBackground)
		_matrix[_answerLine][_answerColumn].background = ofColor(COLORS[goodOne]);
	else
		_matrix[_answerLine][_answerColumn].color = ofColor(COLORS[goodOne]);
	_matrix[_answerLine][_answerColumn].name = string(NAMES[goodOne]);
}

//--------------------------------------------------------------
MiniGame* KoloruSuicchi::clone() {
	KoloruSuicchi* toClone = new KoloruSuicchi();
	toClone->_level = _level;
	toClone->_matrixSize = _matrixSize;
	toClone->_availableColors = _availableColors;
	toClone->_answerLine = _answerLine;
	toClone->_answerColumn = _answerColumn;
	toClone->_matchBackground = _matchBackground;

	toClone->_matrix = new SquareColor*[_matrixSize];
	for(int i=0; i<_matrixSize ; i++) {
		toClone->_matrix[i] = new SquareColor[_matrixSize];
		for(int j=0; j<_matrixSize ; j++) {
			toClone->_matrix[i][j].color = ofColor(_matrix[i][j].color);
			toClone->_matrix[i][j].name = string(_matrix[i][j].name);
			toClone->_matrix[i][j].background = ofColor(_matrix[i][j].background);
		}
	}
	return toClone;
}

//--------------------------------------------------------------
KoloruSuicchi::~KoloruSuicchi() {
	for(int i = 0;i<_matrixSize;i++)
		delete _matrix[i];
}

//--------------------------------------------------------------
void KoloruSuicchi::setup(WM* wiimote) {
	_wiimote = wiimote;
	_background.loadImage(PATH+"sun.png");
	_solved=false;
	switch(_matrixSize) {
	case 4: _visitor.loadFont("fonts/visitor2.ttf",17,false); break;
	case 5: _visitor.loadFont("fonts/visitor2.ttf",15,false); break;
	case 6: _visitor.loadFont("fonts/visitor2.ttf",12,false); break;
	case 7: _visitor.loadFont("fonts/visitor2.ttf",10,false); break;
	case 8: _visitor.loadFont("fonts/visitor2.ttf",9,false); break;
	default: break;
	}
	_pauseStartingTime=0;
	_cursorImage.loadImage(PATH+"fingerpoint.png");
	_switchStart = 0;
	_switchLength = .5 + (_level/3.0);
	_failScreen.init(PATH+"redcross.png",0xffffff);
	_cursorLine = 0;
	_cursorColumn = 0;
}

//--------------------------------------------------------------
void KoloruSuicchi::update() {
	float elapsedTime = ofGetElapsedTimef();
	if(_failScreen.isFinished()) {
		_wiimote->SetRumble(false);	
	};
	//-------------------------------------------------------------------------------------------------------------------
	// WIIMOTE CONTROLS
	//-------------------------------------------------------------------------------------------------------------------
	if(_wiimote->pressed(0, _wiimote->Button.Two())) {
			if(_cursorLine==_answerLine && _cursorColumn==_answerColumn) {
				_solved = true;
			}
			else {
				_failScreen.start(2000);
				_wiimote->SetRumble(true);
			}
	}

	if(_wiimote->pressed(1,_wiimote->Button.Up()))
			leftMove();
	if(_wiimote->pressed(2,_wiimote->Button.Down()))
			rightMove();
	if(_wiimote->pressed(3,_wiimote->Button.Left()))
			downMove();
	if(_wiimote->pressed(4,_wiimote->Button.Right()))
			upMove();

	//-------------------------------------------------------------------------------------------------------------------
	// SWITCH
	//-------------------------------------------------------------------------------------------------------------------
	if(_matchBackground) {
		if(elapsedTime > _switchStart+_switchLength) {
			for(int i =0 ; i< _matrixSize ; i++) {
				for(int j= 0; j < _matrixSize ; j++) {	
					_matrix[i][j].color = ofColor(COLORS[(int) ofRandom(_availableColors)]);
				}
			}
			_switchStart = elapsedTime;
		}
	}
	else {
		if(elapsedTime > _switchStart+_switchLength) {
			for(int i =0 ; i< _matrixSize ; i++) {
				for(int j= 0; j < _matrixSize ; j++) {	
					_matrix[i][j].background = ofColor(COLORS[(int) ofRandom(_availableColors)]);
				}
			}
			_switchStart = elapsedTime;
		}
	}
	
}

//--------------------------------------------------------------
void KoloruSuicchi::draw(int sizeBox) {
	_background.draw(0,0,0,sizeBox,sizeBox);

	float borderDist = sizeBox/16.0;
	float coeffInterval = 0.25;
	float sizeSquares = (float) (sizeBox - 2.0*borderDist) / (_matrixSize+(_matrixSize-1.0)*coeffInterval);
	float sizeInterval = coeffInterval*sizeSquares;

	const static float scale = sizeBox/400.0;

	ofPushMatrix();
	ofEnableAlphaBlending();

	float xDrawing = borderDist;
	float yDrawing = borderDist;
	for(int i =0 ; i< _matrixSize ; i++) {
		for(int j= 0; j < _matrixSize ; j++) {			
			ofPushStyle();
			ofSetColor(_matrix[i][j].background);
			ofRect(xDrawing,yDrawing,0,sizeSquares,sizeSquares);
			ofSetColor(_matrix[i][j].color);
			
			ofPushMatrix();
			ofTranslate(xDrawing, yDrawing);
			ofScale(scale,scale);
			ofRectangle sizeText = _visitor.getStringBoundingBox(_matrix[i][j].name,0,0);
			_visitor.drawString(_matrix[i][j].name,(sizeSquares - sizeText.width*scale) / (2.0*scale),sizeSquares / 2/scale);
			ofPopMatrix();
			ofPopStyle();
			xDrawing += sizeInterval+sizeSquares;
		}
		xDrawing = borderDist;
		yDrawing += sizeInterval+sizeSquares;
	}	
	_cursorImage.setAnchorPercent(0.5,0.5);
	_cursorImage.draw(borderDist+_cursorColumn*(sizeSquares+sizeInterval)+(5*sizeSquares/6),(5*sizeSquares/6)+borderDist+_cursorLine*(sizeSquares+sizeInterval),0,(sizeSquares/2),(sizeSquares/2));

	ofDisableAlphaBlending();
	ofPopMatrix();

	//-------------------------------------------------------------------------------------------------------------------
	// HELPER
	//-------------------------------------------------------------------------------------------------------------------
	ofPushMatrix();
	ofTranslate(sizeBox-(3.0*borderDist/4.0), sizeBox-(3.0*borderDist/4.0));
	ofPushStyle();

	if(_matchBackground)
		ofSetColor(0,0,0);
	else
		ofSetColor(255,255,255);
	ofFill();
	ofRect(0,0,0,borderDist/2.0,borderDist/2.0);
	
	if(_matchBackground)
		ofSetColor(255,255,255);
	else
		ofSetColor(0,0,0);
	ofRect(borderDist/8.0,borderDist/4.0,0,borderDist/10.0, borderDist/10.0);
	ofRect(borderDist/4.0,borderDist/4.0,0,borderDist/10.0, borderDist/10.0);

	ofPopStyle();
	ofPopMatrix();

	
	//-------------------------------------------------------------------------------------------------------------------
	// FAIL SCREEN
	//-------------------------------------------------------------------------------------------------------------------
	_failScreen.draw(sizeBox);
}

//--------------------------------------------------------------
bool KoloruSuicchi::solved() { return _solved; }

//--------------------------------------------------------------
void KoloruSuicchi::pause() {
	_wiimote->SetRumble(false);
	_pauseStartingTime = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void KoloruSuicchi::resume() {
	float elapsedTime = ofGetElapsedTimef() - _pauseStartingTime;
	_switchStart+=elapsedTime;
}

//--------------------------------------------------------------
void KoloruSuicchi::leftMove() {
	//hypercylindricity
	_cursorColumn--;
	if(_cursorColumn < 0)
		_cursorColumn = _matrixSize-1;
}

//--------------------------------------------------------------
void KoloruSuicchi::rightMove() {
	//hypercylindricity
	_cursorColumn++;
	if(_cursorColumn > _matrixSize-1)
		_cursorColumn = 0;
}

//--------------------------------------------------------------
void KoloruSuicchi::downMove() {
	//hypercylindricity
	_cursorLine++;
	if(_cursorLine > _matrixSize-1)
		_cursorLine = 0;
}

//--------------------------------------------------------------
void KoloruSuicchi::upMove() {
	//hypercylindricity
	_cursorLine--;
	if(_cursorLine < 0)
		_cursorLine = _matrixSize-1;
}