#include <iostream>
#include "WM.h"

//--------------------------------------------------------------
WM::WM(){
	updated = false;
}

//--------------------------------------------------------------
void WM::init() {
	Button.Bits = &remote.Button.Bits;
	Acceleration.x = &remote.Acceleration.X;
	Acceleration.y = &remote.Acceleration.Y;
	Acceleration.z = &remote.Acceleration.Z;
}

//--------------------------------------------------------------
bool WM::open() {
	char count = 0;
	while(!remote.Connect(wiimote::FIRST_AVAILABLE)) {
		Sleep(500);
		if(count++ >= 4)
			return false;
	}
	remote.SetLEDs(0x0);
	return true;
}

//--------------------------------------------------------------
void WM::update() {
	updated = (remote.RefreshState()  != NO_CHANGE);
}

//--------------------------------------------------------------
bool WM::isStateNew() {
	return updated;
}

//--------------------------------------------------------------
void WM::enableAccCapture() {
	remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);
}

//--------------------------------------------------------------
void WM::disableAccCapture() {
	remote.SetReportType(wiimote::IN_BUTTONS);
}

//--------------------------------------------------------------
void WM::setLEDs(bool l1, bool l2, bool l3, bool l4) {
	int leds = 0x0;

	if(l1)
		leds |= 0x1;
	if(l2)
		leds |= 0x2;
	if(l3)
		leds |= 0x4;
	if(l4)
		leds |= 0x8;

	remote.SetLEDs((BYTE)leds);
}

//--------------------------------------------------------------
bool WM::connectionLost() {
	return remote.ConnectionLost();
}

//--------------------------------------------------------------
void WM::SetRumble(bool b) {
	remote.SetRumble(b);
}

//--------------------------------------------------------------
int WM::batteryPercent(bool b) {
	return remote.BatteryPercent;
}

//--------------------------------------------------------------
bool WM::pressed(int id, bool pressed) {
	if(pressed && !(_map_pressed & 1<<id)) { _map_pressed |= 1<<id; return true; }
	if(!pressed && (_map_pressed & 1<<id)) { _map_pressed &= ~(1<<id); }
	return false;
}

//--------------------------------------------------------------
bool WM::released(int id, bool pressed) {
	if(pressed && !(_map_released & 1<<id)) { _map_released |= 1<<id; }
	if(!pressed && (_map_released & 1<<id)) { _map_released &= ~(1<<id); return true; }
	return false;
}

//--------------------------------------------------------------
void WM::close() {
	remote.Disconnect();
}