#pragma once

#include "ofMain.h"
#include "WM.h"
#include "LockScreen.h"

#include "Rotator.h"
#include "MemWay.h"
#include "KoloruSuicchi.h"
#include "RocketShake.h"
#include "MathRush.h"
#include "PuzzleLeagueLike.h"

#include "SkillManager.h"

#define PATH string("GameManager/")

//Template class that manages the mini-games
template <typename T>
 class GameManager {
	private:
		//Gamemanager attributes
		//---------------------------------------------------------------------------------
		//Holds all the levels of the T mini-game
	 	T** _levels;
		//The size of the previous attribute
		short _numberLevel;
		//The current level
		short _currentLevel;
		//The controller
		WM* _wiimote;
		//The related skill manager
		SkillManager* _skillManager;
		
		//Graphics
		//---------------------------------------------------------------------------------
		//The level displayed in the HUD
		string _levelString;
		//The lockscreen of the power/skill
		LockScreen _lock;
		//The sprite of the green validation 
		QuickSprite _next;
		//The sprite of the power/skill minus2
		QuickSprite _previous;
		//The blue circle of validation when it is all done
		QuickSprite _done;
		//The police used when writing textual informations
		ofTrueTypeFont _visitor;
		//Draw the HUD (informations in the foreground)
		void drawHUD(int size) {
			ofPushStyle();
			ofEnableAlphaBlending();
			ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
			ofSetColor(0, 0, 0);
			ofRect(0, size-20, 100, 20);
			ofSetColor(200, 200, 200);
			_visitor.drawString(_levelString, 2, size-2);
			ofDisableAlphaBlending();
			ofPopStyle();
		}
	public:
		//Constructors and destructor
		//---------------------------------------------------------------------------------
		//Default constructor : do nothing
		GameManager() { }
		
		//Create a GameManager with the provided starting level and number of levels
		GameManager(unsigned short startingLevel, unsigned short numberofLevel) {
			_numberLevel = numberofLevel;
			_levels = new T*[_numberLevel];
			for(int i=0 ; i<_numberLevel ; i++) {
				_levels[i] = new T(i+startingLevel);
			}
		}
		
		//Destructor
		virtual ~GameManager() {
			delete[] _levels;
		}

		//Create a clone of the object
		GameManager<T>* clone() {
			GameManager<T>* toClone = new GameManager<T>();
			toClone->_numberLevel = _numberLevel;
			toClone->_levels = new T*[_numberLevel];
			for(int i=0 ; i<_numberLevel ; i++) {
				toClone->_levels[i] = _levels[i]->clone();
			}

			return toClone;
		}
		
		//Initialize the variables
		void setup(WM* wiimote, SkillManager* sm) {
			_wiimote = wiimote;
			_skillManager = sm;

			_visitor.loadFont("fonts/visitor2.ttf",32, false);
			_next.init(PATH+"validate.png", 0xffffff);
			_previous.init(PATH+"unvalidate.png", 0xffffff);
			_done.init(PATH+"done.png", 0xcceeff);

			_currentLevel = 0;
			_levels[_currentLevel]->setup(wiimote);

			_levelString = "LVL 1";
		}

		//Update the state of the GameManager
		void update() {
			if(!solved() && !_lock.isActive() && !_next.isActive() && !_previous.isActive()) {
				if(_levels[_currentLevel]->solved()) {
					if(!_next.isActive())
						nextLevel();
				} else {
					_levels[_currentLevel]->update();
				}
			}

			if (_lock.isFinished())
				resume();

			if(_next.isFinished()) {
				if(solved()) {
					_done.start(0);
				}
			}
			_previous.isFinished();
			
		}

		//Draw the GameManager depending of the provided window size
		void draw(int size) {
			if(!_next.isActive() && !solved() && !_lock.isActive() && !_previous.isActive())
				_levels[_currentLevel]->draw(size);

						
			_next.draw(size);
			_done.draw(size);
			_previous.draw(size);
			_lock.draw(size);

			if(!solved())
				drawHUD(size);
		}

		//True if the GameManager is all done
		bool solved() {
			return (_numberLevel<=_currentLevel);
		}

		//Lock the GameManager for an amount oftime
		void lock(int duration) {
			pause();
			_lock.start(duration);
		}
		
		//Unlock the GameManager before its theorical end
		void unlock() {
			resume();
			_lock.stop();
		}

		//Pause the GameManager
		void pause() {
			if(!solved()) {
				_levels[_currentLevel]->pause();
			}
		}
		
		//Resume the GameManager
		void resume() {
			if(!solved()) {
				_levels[_currentLevel]->resume();
			}
		}

		//Change the actual level for the next one
		void nextLevel() {
			_skillManager->addLevel((int)this);
			_currentLevel++;
			if(!solved())
				_levels[_currentLevel]->setup(_wiimote);
			stringstream st;
			st << "LVL " << (_currentLevel+1); //index 0 is level 1.
			_levelString = st.str();
			_next.start(1000);
		}

		//Change the actual level for the previous one, used when a skill is used
		void previousLevel() {
			_skillManager->removeLevel();
			_done.stop();
			_currentLevel --;
			if(_currentLevel < 0)
				_currentLevel = 0;
			_levels[_currentLevel]->setup(_wiimote);
			stringstream st;
			st << "LVL " << (_currentLevel+1); //index 0 is level 1.
			_levelString = st.str();
			_previous.start(1000);
		}
};
