#include "RushMyCube.h"

//--------------------------------------------------------------
void RushMyCube::setup(){
	ofBackground(0,0,0);
	ofSetWindowTitle("RushMyCube");

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	short numPlayers = 0;
	int i;

	list<Cube*>::iterator it;

	for(i=0;i<MAX_PLAYERS;i++) {
		remote[i] = new WM();
		remote[i]->init();
		cout << "Wiimote " << i+1 << ": ";
		if(!remote[i]->open()) { cout << "time out\n"; break; }
		cout << "connected\n"; numPlayers++;
	}

	if(numPlayers == 0) {
		cout << "\nNo Wiimote connected !\nExiting...\n";
		system("pause");
		std::exit(0);
	}

	string input;
	unsigned short startingLevel;
	cout << endl << "Starting Level [3]: ";
	getline(cin, input);
	if(input.empty())
		startingLevel = 3;
	else
		startingLevel = atoi(input.c_str());
	if(startingLevel > 24)
		startingLevel = 24;
	unsigned short numberofLevel;
	cout << "Levels per face [4]: ";
	getline(cin, input);
	if(input.empty())
		numberofLevel = 4;
	else
		numberofLevel = atoi(input.c_str());
	if(numberofLevel > 28)
		numberofLevel = 28;

	
	for(i=0;i<numPlayers;i++) {
		if(i==0)
			cubes.push_back(new Cube(startingLevel, numberofLevel));
		else 
			cubes.push_back(cubes.front()->clone());
	}

	for(i=0, it = cubes.begin(); it != cubes.end(); it++, i++) {
		(*it)->setup(remote[i], &cubes);
		cam[i] = new ofCamera();
		cam[i]->lookAt(ofVec3f(0, 0, 0));
	}

	lag.loadImage("skills/lag.png");
	lock.loadImage("skills/lock.png");
	plus1.loadImage("skills/plus1.png");
	minus2.loadImage("skills/minus2.png");
	move.loadImage("skills/move.png");
	winFont.loadFont("fonts/visitor2.ttf", 80, false);
	winner = 0;
	sw.start();
}

//--------------------------------------------------------------
void RushMyCube::update() {
	if(winner) {
		if(_finish.newTop()) {
			cam[winner-1]->rotateAround(_finish.getCurrentTop()/4,ofVec3f(0, 1, 0), ofVec3f(0, 0, 0));
			cam[winner-1]->truck(30);
			cam[winner-1]->lookAt(ofVec3f(0, 0, 0));
		}
	}
	else {
		sw.update();
		int i;
		list<Cube*>::iterator it;
		for(it = cubes.begin(), i=0; it != cubes.end(); it++, i++) {
			remote[i]->update();
			(*it)->update();
			if((*it)->solved()) {
				winner = i+1;
				cam[i]->boom(700);
				sw.pause();
				_finish.start(3000, 90);
			}
		}
	}
}

//--------------------------------------------------------------
void RushMyCube::draw() {
	int i;
	list<Cube*>::iterator it;
	for(it = cubes.begin(), i=0; it != cubes.end(); it++, i++) {
		if(!winner) {
			cam[i]->begin(ofRectangle(camWidth*i,0,camWidth,drawSize));
			(*it)->draw(drawSize*.9);
			cam[i]->end();
			ofPushMatrix();
			ofSetColor(255, 50, 0);
			ofRect(4+camWidth*i, 4, 42, 42);
			ofSetColor(255);
			ofEnableAlphaBlending();
			for(list<SkillManager::Skills>::iterator its= (*it)->skillManager->skills.begin(); its != (*it)->skillManager->skills.end(); ++its) {
				ofImage *toDraw;
				switch(*its) {
					case 0: toDraw = &lag; break;
					case 1: toDraw = &move; break;
					case 2: toDraw = &lock; break;
					case 3: toDraw = &plus1; break;
					case 4: toDraw = &minus2; break;
				}

				toDraw->draw(5+camWidth*i, 5, 40, 40);
				ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
				ofTranslate(45, 0);
			}
			ofDisableAlphaBlending();
			ofPopMatrix();
		}
		else {
			if(winner == i+1) {
				cam[i]->begin(ofRectangle(camWidth*i,0,camWidth,drawSize));
					(*it)->drawAllFaces(drawSize*.9);
				cam[i]->end();
				ofSetColor(120, 120, 255);
				winFont.drawString("YOU WIN", camWidth*i  + (camWidth - 342)/2, 60);
				ofSetColor(255);
			}
		}
	}

	sw.draw(drawSize, ofGetWidth()/2, ofGetHeight()-40);
}

//--------------------------------------------------------------
void RushMyCube::keyPressed(int key){
	if(key == 'f')
		ofToggleFullscreen();
}

//--------------------------------------------------------------
void RushMyCube::keyReleased(int key){

}

//--------------------------------------------------------------
void RushMyCube::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void RushMyCube::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void RushMyCube::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void RushMyCube::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void RushMyCube::windowResized(int w, int h){
	
	 drawSize = min(w/(int)cubes.size(), h-50);
	 camWidth = w / cubes.size();
	 for(int i=0;i<cubes.size();i++) {
		cam[i]->setPosition(0, 0, 1.36*drawSize);
	}
}

//--------------------------------------------------------------
void RushMyCube::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void RushMyCube::dragEvent(ofDragInfo dragInfo){ 

}
