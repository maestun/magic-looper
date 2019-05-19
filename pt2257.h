/*
 * Volume control using PT2257 chip
 * --------------------------------
 * Uses a linear pot, pin wiring (pot facing) is:
 * 1 => Vcc
 * 2 => aAnalogInputPin
 * 3 => GND
 * 
 * Used Arduino Wire library, connect SDA to A4 and SCL to A5.
 */

#ifndef __PT2257_H__
#define __PT2257_H__


#include <Arduino.h>
#include "button.h"


class PT2257 : public ButtonListener {
private:
	uint8_t		attenuation_min;
	uint8_t		attenuation_max;
	uint16_t	analog_min;
	uint16_t	analog_max;
	uint8_t		analog_pin;
	Button *	mute_button;    
	void        onButtonEvent(uint8_t aPin, EButtonScanResult aResult);
	void 		update(int aAnalogValue);
public:
	PT2257(uint8_t aAnalogInputPin);
	PT2257(uint8_t aAnalogInputPin, int8_t aMutePin);
	PT2257(uint8_t aAnalogInputPin, int8_t aMutePin, uint8_t aMinAttn, uint8_t aMaxAttn);
	void mute(bool aMute);
	void disable();
	void scan();
};

#endif // __PT2257_H__
