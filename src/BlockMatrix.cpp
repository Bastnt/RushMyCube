#include "BlockMatrix.h"

//--------------------------------------------------------------
BlockMatrix::BlockMatrix() {
}

//--------------------------------------------------------------
BlockMatrix::BlockMatrix(int level) {
	_victoriousHeight = 7 - level*1.5;
	levelGenerator();
}

//--------------------------------------------------------------
void BlockMatrix::setup() {
	PuzzleRow::initImages();
}

//--------------------------------------------------------------
void BlockMatrix::levelGenerator() {
	PuzzleRow* oldest = new PuzzleRow();
	oldest->filledRow();
	_rows[0] = oldest;

	PuzzleRow* old = new PuzzleRow();
	old->filledRow();
	_rows[1] = old;

	PuzzleRow* actual = new PuzzleRow();
	for(int i = 2 ; i < STARTING_HEIGHT ; i++) {
		do {
			actual->filledRow();
		}
		while(actual->hasVerticalTriplet(*old, *oldest));
		_rows[i] = new PuzzleRow(*actual);
		oldest = new PuzzleRow(*old);
		old = new PuzzleRow(*actual);
	}
	for(int i = STARTING_HEIGHT ; i< MAX_HEIGHT ; i++) {
		actual->emptyRow();
		_rows[i] = new PuzzleRow(*actual);
	}
	free(old);
	free(oldest);
	free(actual);
}

//--------------------------------------------------------------
void BlockMatrix::newLine() {
	if(_rows[MAX_HEIGHT-1]->isEmpty()){
		for(int i = MAX_HEIGHT-1 ; i > 0 ; i--) {
			_rows[i] = _rows[i-1];
		}
		PuzzleRow* newRow = new PuzzleRow();
		newRow->filledRow();
		_rows[0]= new PuzzleRow(*newRow);
		for(int i=0 ; i<6 ; i++) {
			erase(0,i);
		}
		_victoriousHeight++;
	}
}

//--------------------------------------------------------------
BlockMatrix::BlockMatrix(const BlockMatrix& toCopy) {
	_victoriousHeight = toCopy._victoriousHeight;
	for(int i = 0; i < MAX_HEIGHT ; i++) {
		_rows[i] = new PuzzleRow(*toCopy._rows[i]);
	}
}

//--------------------------------------------------------------
BlockMatrix::~BlockMatrix() {
	for(int i=0 ; i<MAX_HEIGHT ; i++) {
		delete _rows[i];
	}
	delete[] _rows;
}

//--------------------------------------------------------------
void BlockMatrix::update(){
	float animationPitch = (ofGetElapsedTimeMillis() - elapsedTime) / 3.0;
	elapsedTime= ofGetElapsedTimeMillis();

	for(int row = 0 ; row < MAX_HEIGHT ; row++ ) {
		for (int column = 0 ; column <6 ; column++) {
			if(_rows[row]->_blocks[column].animated && _rows[row]->_blocks[column].animationPercent < 100) 
				_rows[row]->_blocks[column].animationPercent += animationPitch;
			if(_rows[row]->_blocks[column].animationPercent >= 100)
				endGravity(row,column);
			if(!_rows[row]->_blocks[column].animated && row>0 && _rows[row-1]->_blocks[column].color==empty && _rows[row]->_blocks[column].color!=empty)
				gravity(row,column);
		}	
	}
}

//--------------------------------------------------------------
void BlockMatrix::endGravity(int row,int column) {
	if(row >0) {
		if(_rows[row-1]->_blocks[column].color==empty && _rows[row]->_blocks[column].color!=empty) {//continuing case
			ColorBlock temp = _rows[row]->_blocks[column].color;
			_rows[row]->_blocks[column].color = _rows[row-1]->_blocks[column].color;
			_rows[row-1]->_blocks[column].color = temp;
			_rows[row]->_blocks[column].locked=true;
			_rows[row-1]->_blocks[column].animated=true;
			_rows[row-1]->_blocks[column].animationPercent=0;
		}
		else if(!_rows[row-1]->_blocks[column].animated) {//ending case
			_rows[row]->_blocks[column].animated=false;
			_rows[row]->_blocks[column].locked=false;
			_rows[row]->_blocks[column].animationPercent=0;
			erase(row,column);
		}
		if(row < MAX_HEIGHT-1) {
			_rows[row+1]->_blocks[column].locked=false;
			if(_rows[row+1]->_blocks[column].color==empty)
				_rows[row+1]->_blocks[column].animated=false;
		}
	}
	else{//bottom ending case
		_rows[row]->_blocks[column].animated=false;
		_rows[row]->_blocks[column].animationPercent=0;
		_rows[row+1]->_blocks[column].locked=false;
		erase(row,column);
	}
}

//--------------------------------------------------------------
void BlockMatrix::gravity(int row, int column){
	if(!_rows[row-1]->_blocks[column].animated) {
		ColorBlock temp = _rows[row]->_blocks[column].color;
		_rows[row]->_blocks[column].color = _rows[row-1]->_blocks[column].color;
		_rows[row-1]->_blocks[column].color = temp;
		_rows[row]->_blocks[column].locked=true;
		_rows[row-1]->_blocks[column].animated=true;
		_rows[row-1]->_blocks[column].animationPercent=0;
	}
}

//--------------------------------------------------------------
void BlockMatrix::draw(int size){
	float sizeRow = size / 2.0;
	float borderDistX = (size - sizeRow) / 2.0;
	float sizeBlock = sizeRow / 6.7;
	float sizeInterval = sizeBlock / 10.0;
	float borderDistY = (size - (10*sizeBlock + 11*sizeInterval)) / 2.0;

	//-------------------------------------------------------------------------------------------------------------------
	// BLOCKS
	//-------------------------------------------------------------------------------------------------------------------

	ofPushMatrix();
	float xDrawing = borderDistX;
	float yDrawing = size - borderDistY - sizeInterval - sizeBlock/2.0;
	for(int i = 0 ; i < MAX_HEIGHT ; i++ ) {
		ofTranslate(xDrawing,yDrawing);
		_rows[i]->draw(sizeRow);
		ofTranslate(-xDrawing,-yDrawing);
		yDrawing -= (sizeInterval + sizeBlock);
	}
	if(_victoriousHeight >= 0) {
		ofPushStyle();
		ofSetColor(255,0,190);
		ofRect(xDrawing,size - borderDistY - sizeInterval - _victoriousHeight*(sizeInterval+sizeBlock),0,sizeRow,sizeInterval);
		ofPopStyle();
	}
	ofPopMatrix();
}

//--------------------------------------------------------------
bool BlockMatrix::solved(){
	return _victoriousHeight>=0 && _rows[_victoriousHeight]->isEmpty();
}


//--------------------------------------------------------------
void BlockMatrix::exchange(int row, int column){
	if(column < 5 && !_rows[row]->_blocks[column].locked && !_rows[row]->_blocks[column+1].locked && !_rows[row]->_blocks[column].animated && !_rows[row]->_blocks[column+1].animated) {
		_rows[row]->exchange(column);
		if(!(row > 0 && _rows[row-1]->_blocks[column].color==empty))
			erase(row,column);
		if(!(row > 0 && _rows[row-1]->_blocks[column+1].color==empty))
			erase(row,column+1);
	}
}

//--------------------------------------------------------------
void BlockMatrix::erase(int row, int column) {
	ColorBlock color = _rows[row]->_blocks[column].color;
	if(color != empty) {
		//horizontal erase
		bool left = (column > 1 && color==_rows[row]->_blocks[column-1].color && !_rows[row]->_blocks[column-1].animated && !_rows[row]->_blocks[column-1].locked 
						&& color==_rows[row]->_blocks[column-2].color && !_rows[row]->_blocks[column-2].animated && !_rows[row]->_blocks[column-2].locked);
		bool central = (column > 0 && column < 5 && color==_rows[row]->_blocks[column-1].color && !_rows[row]->_blocks[column-1].animated && !_rows[row]->_blocks[column-1].locked 
						&& color==_rows[row]->_blocks[column+1].color && !_rows[row]->_blocks[column+1].animated && !_rows[row]->_blocks[column+1].locked);
		bool right = (column < 4 && color==_rows[row]->_blocks[column+1].color && !_rows[row]->_blocks[column+1].locked && !_rows[row]->_blocks[column+1].animated 
						&& color==_rows[row]->_blocks[column+2].color && !_rows[row]->_blocks[column+2].locked && !_rows[row]->_blocks[column+2].animated);
		if(left) {
			makeEmpty(row,column-1);
			makeEmpty(row,column-2);
		}
		if(central) {
			makeEmpty(row,column-1);
			makeEmpty(row,column+1);
		}
		if(right) {
			makeEmpty(row,column+2);
			makeEmpty(row,column+1);
		}
		//vertical erase
		bool top = (row < MAX_HEIGHT-2 && color==_rows[row+1]->_blocks[column].color && color==_rows[row+2]->_blocks[column].color);
		bool centralV = (row > 0 && row < MAX_HEIGHT-1 && color==_rows[row+1]->_blocks[column].color
						&& color==_rows[row-1]->_blocks[column].color && !_rows[row-1]->_blocks[column].locked && !_rows[row-1]->_blocks[column].animated);
		bool down = (row > 1 && color==_rows[row-1]->_blocks[column].color && !_rows[row-1]->_blocks[column].locked && !_rows[row-1]->_blocks[column].animated 
						&& color==_rows[row-2]->_blocks[column].color && !_rows[row-2]->_blocks[column].locked && !_rows[row-2]->_blocks[column].animated);
		if(top) {
			makeEmpty(row+1,column);
			makeEmpty(row+2,column);
			_rows[row+1]->_blocks[column].locked =_rows[row+1]->_blocks[column].animated=_rows[row+2]->_blocks[column].animated=_rows[row+2]->_blocks[column].animated=false;
		}
		if(centralV) {
			makeEmpty(row+1,column);
			_rows[row+1]->_blocks[column].locked =_rows[row+1]->_blocks[column].animated=false;
			makeEmpty(row-1,column);
		}
		if(down) {
			makeEmpty(row-1,column);
			makeEmpty(row-2,column);
		}
		//square itself
		if(right || left || central || top || down ||centralV)
			makeEmpty(row,column);
	}
}

//--------------------------------------------------------------
void BlockMatrix::makeEmpty(int row, int column) {
	_rows[row]->_blocks[column].color = empty;
}