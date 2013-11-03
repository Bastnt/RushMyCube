#include "PuzzleRow.h"

//--------------------------------------------------------------
ofImage PuzzleRow::_blue;
ofImage PuzzleRow::_yellow;
ofImage PuzzleRow::_green;
ofImage PuzzleRow::_purple;
ofImage PuzzleRow::_red;
bool PuzzleRow::_imagesLoaded = false;

//--------------------------------------------------------------
PuzzleRow::PuzzleRow() {
}

//--------------------------------------------------------------
PuzzleRow::~PuzzleRow() {}

//--------------------------------------------------------------
PuzzleRow::PuzzleRow(const PuzzleRow& toCopy) {
	for(int i = 0 ; i<6 ; i++)
		_blocks[i] = toCopy._blocks[i];
}

//--------------------------------------------------------------
void PuzzleRow::filledRow() {
	int color, oldColor, oldestColor;

	oldestColor = ofRandom(5)+1;
	Block b0;
	b0.color= (ColorBlock) oldestColor;
	b0.locked=false;
	b0.animated=false;
	b0.animationPercent=0;
	_blocks[0]=b0;

	oldColor = ofRandom(5)+1;
	Block b1;
	b1.color = (ColorBlock) oldColor;
	b1.locked=false;
	b1.animated=false;
	b1.animationPercent=0;
	_blocks[1]=b1;

	Block b2;
	b2.locked=false;
	b2.animated=false;
	b2.animationPercent=0;

	for (int i=2; i<6; i++)
	{
		do {
			color = ofRandom(5)+1;
		}
		while ((color == oldColor) && (color == oldestColor));
		b2.color = (ColorBlock) color;
		_blocks[i] = b2;
		oldestColor = oldColor;
		oldColor = color;
	}
}

//--------------------------------------------------------------
void PuzzleRow::emptyRow() {
	for (int i=0; i<6; i++) {
		_blocks[i].color = empty;
		_blocks[i].animationPercent=0;
		_blocks[i].locked=false;
		_blocks[i].animated=false;
	}
}

//--------------------------------------------------------------
bool PuzzleRow::isEmpty() {
	for (int i=0; i<6; i++)
		if(_blocks[i].color!=empty)
			return false; 
	return true;
}

//--------------------------------------------------------------
void PuzzleRow::exchange(int column) {
	if(column < 5) {
		Block temp = _blocks[column+1];
		_blocks[column+1] = _blocks[column];
		_blocks[column] = temp;
	}
}

//--------------------------------------------------------------
void PuzzleRow::initImages() {
	if(!_imagesLoaded) {
	_yellow.loadImage(PATH+"yellow.png");
	_yellow.setAnchorPercent(0.5,0.5);
	_blue.loadImage(PATH+"blue.png");
	_blue.setAnchorPercent(0.5,0.5);
	_red.loadImage(PATH+"red.png");
	_red.setAnchorPercent(0.5,0.5);
	_purple.loadImage(PATH+"purple.png");
	_purple.setAnchorPercent(0.5,0.5);
	_green.loadImage(PATH+"green.png");
	_green.setAnchorPercent(0.5,0.5);
	_imagesLoaded=true;
	}
}

//--------------------------------------------------------------
void PuzzleRow::draw(int sizeRow) {
	//be careful : one row is composed of 6 blocks and 7 intervals
	float sizeBlock = sizeRow / 6.7;
	float intervalBlocks = sizeBlock / 10.0;

	float xDrawing = intervalBlocks+(sizeBlock/2.0);
	ofPushStyle(); //to ignor all previous color settings
	ofEnableAlphaBlending();
	for(int i=0; i<6 ; i++) {
		float yDrawing = 0;
		if(_blocks[i].animated) {
			yDrawing = -((1-(_blocks[i].animationPercent/100.0)) * (sizeBlock+intervalBlocks));
		}
		switch(_blocks[i].color) {
			case empty: break;
				case yellow: _yellow.draw(xDrawing,yDrawing,0,sizeBlock, sizeBlock); break;
				case blue: _blue.draw(xDrawing,yDrawing,0,sizeBlock, sizeBlock); break;
				case green: _green.draw(xDrawing,yDrawing,0,sizeBlock, sizeBlock); break;
				case red: _red.draw(xDrawing,yDrawing,0,sizeBlock, sizeBlock); break;
				case purple: _purple.draw(xDrawing,yDrawing,0,sizeBlock, sizeBlock); break;
		}
		xDrawing += intervalBlocks+sizeBlock;
	}
	ofDisableAlphaBlending();
	ofPopStyle();
}

//--------------------------------------------------------------
bool PuzzleRow::hasVerticalTriplet(PuzzleRow& p1, PuzzleRow& p2) {
	for(int i=0 ; i<6 ; i++) {
		if(_blocks[i].color==p1._blocks[i].color && _blocks[i].color==p2._blocks[i].color)
			return true;
	}
	return false;
}