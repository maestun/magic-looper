#include "pt2257.h"

static const int8_t PIN_NULL = -1;
static const uint8_t PT2257_ADDRESS	= 0x88;
static const uint8_t PT2257_MIN_ATTN = 0;
static const uint8_t PT2257_MAX_ATTN = 79;
static const uint16_t ANALOG_DEFAULT_MIN = 0;
static const uint16_t ANALOG_DEFAULT_MAX = 1023;


PT2257::PT2257(uint8_t aAnalogInputPin) : 
		PT2257(aAnalogInputPin, ANALOG_DEFAULT_MIN, ANALOG_DEFAULT_MAX) {
}

PT2257::PT2257(uint8_t aAnalogInputPin, uint16_t aAnalogMin, uint16_t aAnalogMax) :
		PT2257(aAnalogInputPin, aAnalogMin, aAnalogMax, A4, A5) {
}

PT2257::PT2257(uint8_t aAnalogInputPin, uint16_t aAnalogMin, uint16_t aAnalogMax, int8_t aSDAPin, int8_t aSCLPin) : 
		PT2257(aAnalogInputPin, aAnalogMin, aAnalogMax, PT2257_MIN_ATTN, PT2257_MAX_ATTN, A4, A5) {
}

PT2257::PT2257(uint8_t aAnalogInputPin, uint16_t aAnalogMin, uint16_t aAnalogMax, uint8_t aMinAttn, uint8_t aMaxAttn, int8_t aSDAPin, int8_t aSCLPin) {
	// this chip needs some time to init, block all !
	analog_pin = aAnalogInputPin;
	analog_min = aAnalogMin;
	analog_max = aAnalogMax;
	attenuation_min = aMinAttn;
	attenuation_max = aMaxAttn;
	pinMode(analog_pin, INPUT);
	delay(200);

	softI2C = new SoftI2CMaster(aSDAPin, aSCLPin);

	dprint(F("PT2257 initialized: analog pin "));
	dprint(analog_pin);
	dprint(F(" / analog min "));
	dprint(analog_min);
	dprint(F(" / analog max "));
	dprintln(analog_max);
}


void PT2257::update(int aAnalogValue) {

	uint8_t db_10, db_1, data;
	int db_attenuation;

    softI2C->beginTransmission(PT2257_ADDRESS);
    
    // attenuation goes from -79 to 0 db
    db_attenuation = map(aAnalogValue, analog_min, analog_max, attenuation_min, attenuation_max);
    db_10 = (db_attenuation / 10);
    db_1 = (db_attenuation % 10);
    data = 0;
    
    
    dprint(F("PT2257 analog read: "));
    dprint(aAnalogValue);
    dprint(F(" => attenuation: "));
    dprint(db_attenuation);
    dprintln(F("db"));
    
    
    if(db_10 > 0) {
        data = 0b11100000;   // 0b11100xxx : function "2-Channel, -10dB/step"
        data |= (db_10 & 0b00000111); // last three bytes => attenuation absolute value in decades (0...7)
        softI2C->send(data);
    }
    if(db_1 > 0) {
        data = 0b11010000;   // 0b1101xxxx : function "2-Channel, -1dB/step"
        data |= (db_1 & 0b00001111); // last four bytes => attenuation absolute value in units (0...9)
        softI2C->send(data);
    }
    softI2C->endTransmission();
}


void PT2257::disable() {
    softI2C->beginTransmission(PT2257_ADDRESS);
    // 1    1    1    1    1    1    1    1    Function OFF (-79dB)
    softI2C->send(0b11111111);
    softI2C->endTransmission();
}


void PT2257::scan() {
	update(analogRead(analog_pin));
}
