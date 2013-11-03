#include "PuzzleLeagueLike.h"

//--------------------------------------------------------------
PuzzleLeagueLike::PuzzleLeagueLike() {}

//--------------------------------------------------------------
PuzzleLeagueLike::PuzzleLeagueLike(int level) {
	_matrix = new BlockMatrix(level);
	_startingMatrix = new BlockMatrix(*_matrix);
}



//--------------------------------------------------------------
MiniGame* PuzzleLeagueLike::clone() {
	PuzzleLeagueLike* copy = new PuzzleLeagueLike();
	copy->_matrix = new BlockMatrix(*_matrix);
	copy->_startingMatrix = new BlockMatrix(*_startingMatrix);
	return copy;
}

//--------------------------------------------------------------
PuzzleLeagueLike::~PuzzleLeagueLike() {
	delete _matrix;
}

//--------------------------------------------------------------
void PuzzleLeagueLike::setup(WM* wiimote){
	_background.loadImage(PATH+"fond.png");
	_cursorImage.loadImage(PATH+"cursor.png");
	_cursorImage.setAnchorPercent(0.5,0.5);
	_cursorColumn = 0;
	_cursorRow = 0;
	_startingMovementTime=0;
	_wiimote = wiimote;
	_matrix = new BlockMatrix(*_startingMatrix);
	_matrix->setup();
}

//--------------------------------------------------------------
void PuzzleLeagueLike::update(){
	_matrix->update();

	//-------------------------------------------------------------------------------------------------------------------
	// WIIMOTE CONTROLS
	//-------------------------------------------------------------------------------------------------------------------

	if(_wiimote->pressed(1,_wiimote->Button.Two())) {
		_matrix->exchange(_cursorRow,_cursorColumn);
	}

	if(_wiimote->pressed(2,_wiimote->Button.One())) {
		_matrix->newLine();
	}

	if(_wiimote->Button.Up())
		if(allowMoving())
			leftMove();
	if(_wiimote->Button.Down())
		if(allowMoving())
			rightMove();
	if(_wiimote->Button.Left())
		if(allowMoving())
			downMove();
	if(_wiimote->Button.Right())
		if(allowMoving())
			upMove();
	
	if(_wiimote->pressed(0, _wiimote->Button.Up() || _wiimote->Button.Down() || _wiimote->Button.Left() || _wiimote->Button.Right()))
		_startingMovementTime += BUTTON_INTERVAL*2;
	
	if(_wiimote->released(0, _wiimote->Button.Up() || _wiimote->Button.Down() || _wiimote->Button.Left() || _wiimote->Button.Right()))
		_startingMovementTime = 0;

}

//--------------------------------------------------------------
void PuzzleLeagueLike::draw(int size){
	float sizeRow = size / 2.0;
	float borderDistX = (size - sizeRow) / 2.0;
	float sizeBlock = sizeRow / 6.7;
	float sizeInterval = sizeBlock / 10.0;
	float borderDistY = (size - (10*sizeBlock + 11*sizeInterval)) / 2.0;
	//-------------------------------------------------------------------------------------------------------------------
	// BACKGROUND
	//-------------------------------------------------------------------------------------------------------------------
	
	ofPushStyle();
	ofEnableAlphaBlending();
	_background.draw(0,0,0,size,size);
	ofDisableAlphaBlending();
	ofSetColor(255,127,0);
	ofRect(borderDistX-sizeInterval,borderDistY,0,sizeRow+sizeInterval*2,(10*sizeBlock + 12*sizeInterval));
	ofSetColor(128,0,128);
	ofRect(borderDistX-1-sizeInterval,borderDistY-1,0,2+sizeRow+sizeInterval*2,2+(10*sizeBlock + 12*sizeInterval));
	ofSetColor(0,0,0);
	ofRect(borderDistX+1-sizeInterval,borderDistY+1,0,sizeRow-2+sizeInterval*2,(10*sizeBlock + 12*sizeInterval)-2);
	ofPopStyle();


	//-------------------------------------------------------------------------------------------------------------------
	// MATRIX
	//-------------------------------------------------------------------------------------------------------------------
	_matrix->draw(size);

	//-------------------------------------------------------------------------------------------------------------------
	// CURSOR
	//-------------------------------------------------------------------------------------------------------------------
	ofEnableAlphaBlending();
	_cursorImage.draw(borderDistX + sizeInterval/2 + (_cursorColumn+1)*(sizeBlock+sizeInterval),
					  size - borderDistY - sizeInterval - sizeBlock/2.0 - _cursorRow*(sizeBlock+sizeInterval) ,0,(sizeBlock+sizeInterval)*2+sizeInterval,sizeBlock+sizeInterval);
	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
bool PuzzleLeagueLike::solved(){
	return _matrix->solved();
}

//--------------------------------------------------------------
void PuzzleLeagueLike::pause(){}

//--------------------------------------------------------------
void PuzzleLeagueLike::resume(){}

//--------------------------------------------------------------
bool PuzzleLeagueLike::allowMoving() {
	if((long long)(ofGetElapsedTimeMillis() - _startingMovementTime) > BUTTON_INTERVAL) {
		_startingMovementTime = ofGetElapsedTimeMillis();
		return true;
	}
	return false;
}

//--------------------------------------------------------------
void PuzzleLeagueLike::leftMove() {
	if(_cursorColumn>0)
		_cursorColumn--;
}

//--------------------------------------------------------------
void PuzzleLeagueLike::rightMove() {
	if(_cursorColumn<4)
	_cursorColumn++;
}

//--------------------------------------------------------------
void PuzzleLeagueLike::upMove() {
	if(_cursorRow < BlockMatrix::MAX_HEIGHT-1)
	_cursorRow++;
}

//--------------------------------------------------------------
void PuzzleLeagueLike::downMove() {
	if(_cursorRow>0)
	_cursorRow--;
}