#pragma once
#include "wiimote.h"

class WM {
	private:
		//Nested classes
		//---------------------------------------------------------------------------------
			class button {
				public:
					//Hold the bits map of the pressed buttons
					WORD *Bits;

					//Enumeration using the previous map to isolate buttons
					enum mask {
					LEFT	= 0x0001,
					RIGHT	= 0x0002,
					DOWN	= 0x0004,
					UP		= 0x0008,
					PLUS	= 0x0010,
					TWO		= 0x0100,
					ONE		= 0x0200,
					_B		= 0x0400,
					_A		= 0x0800,
					MINUS	= 0x1000,
					HOME	= 0x8000,
					ALL		= LEFT|RIGHT|DOWN|UP|PLUS|TWO|ONE|_A|_B|MINUS|HOME,
					};

					//Getters holding informations of the pressed buttons
					inline bool A		() const	{ return (*Bits & _A)    != 0; }
					inline bool B		() const	{ return (*Bits & _B)	!= 0; }
					inline bool Plus	() const	{ return (*Bits & PLUS)  != 0; }
					inline bool Home	() const	{ return (*Bits & HOME)  != 0; }
					inline bool Minus	() const	{ return (*Bits & MINUS) != 0; }
					inline bool One		() const	{ return (*Bits & ONE)   != 0; }
					inline bool Two		() const	{ return (*Bits & TWO)   != 0; }
					inline bool Up		() const	{ return (*Bits & UP)	!= 0; }
					inline bool Down	() const	{ return (*Bits & DOWN)  != 0; }
					inline bool Left	() const	{ return (*Bits & LEFT)  != 0; }
					inline bool Right	() const	{ return (*Bits & RIGHT) != 0; }
			};

			class acceleration {
				public:
					//Values of the accelerations of the matching axis
					//---------------------------------------------------------------------------------
					float *x, *y, *z;

					//Getters
					//---------------------------------------------------------------------------------
					inline float X() const { return *x; }
					inline float Y() const { return *y; }
					inline float Z() const { return *z; }
			};

		//Attributes
		//---------------------------------------------------------------------------------
		//The Wiimote
		wiimote remote;
		//True if the Wiimote is untouched
		bool updated;
		//Hold informations of the pressed buttons
		unsigned short _map_pressed;
		//Hold informations of the released buttons
		unsigned short _map_released;
	public:
		//Members
		//---------------------------------------------------------------------------------
		//Hold informations of the buttons
		button Button;
		//Hold informations of the accelerations
		acceleration Acceleration;
		
		//Constructor
		//---------------------------------------------------------------------------------
		//Empty constructor : do (almost) nothing
		WM();
		
		//Hardware related methods
		//---------------------------------------------------------------------------------
		//Initialization method
		void init();
		//Finding and opening hardware method
		bool open();
		//Closing hardware method
		void close();
		//Update the state of the buttons
		void update();
		//True if the connection was lost with the hardware
		bool connectionLost();
		
		//Getters and setters
		//---------------------------------------------------------------------------------
		//Getter of updated attribute
		bool isStateNew();
		//Enable gathering of acceleration datas (and IR datas)
		void enableAccCapture();
		//Disable gathering of acceleration datas (and IR datas)
		void disableAccCapture();
		//Hold informations of the pressed buttons
		bool pressed(int id, bool pressed);
		//Hold informations of the released buttons
		bool released(int id, bool pressed);
		//Set the LEDs to on or off
		void setLEDs(bool l1=0, bool l2=0, bool l3=0, bool l4=0);
		//Set the vibration to on or off
		void SetRumble(bool b);
		//Get the battery state
		int batteryPercent(bool b);
};