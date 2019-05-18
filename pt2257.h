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
	uint16_t		analog_min_value;
	uint16_t		analog_max_value;
	uint8_t			analog_pin;
	SoftI2CMaster *	softI2C;
	void 			update(int aAnalogValue);
public:
	PT2257(uint8_t aAnalogInputPin);
	PT2257(uint8_t aAnalogInputPin, uint16_t aAnalogMin, uint16_t aAnalogMax);
	PT2257(uint8_t aAnalogInputPin, uint16_t aAnalogMin, uint16_t aAnalogMax, int8_t aSDAPin, int8_t aSCLPin);
	void disable();
	void scan();
};