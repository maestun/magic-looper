/*
 * Volume control using PT2257 chip
 * --------------------------------
 * Uses a linear pot, pin wiring (pot facing) is:
 * 1 => Vcc
 * 2 => aAnalogInputPin
 * 3 => GND
 *
 */
#include <Arduino.h>
#include "debug.h"
#include "SoftI2CMaster.h"

class PT2257 {
private:
	uint8_t			attenuation_min;
	uint8_t			attenuation_max;
	uint16_t		analog_min;
	uint16_t		analog_max;
	uint8_t			analog_pin;
	SoftI2CMaster *	softI2C;
	void 			update(int aAnalogValue);
public:
	PT2257(uint8_t aAnalogInputPin);
	PT2257(uint8_t aAnalogInputPin, uint16_t aAnalogMin, uint16_t aAnalogMax);
	PT2257(uint8_t aAnalogInputPin, uint16_t aAnalogMin, uint16_t aAnalogMax, int8_t aSDAPin, int8_t aSCLPin);
	PT2257(uint8_t aAnalogInputPin, uint16_t aAnalogMin, uint16_t aAnalogMax, uint8_t aMinAttn, uint8_t aMaxAttn, int8_t aSDAPin, int8_t aSCLPin);
	void disable();
	void scan();
};