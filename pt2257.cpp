#include "pt2257.h"
#include "debug.h"
#include <Wire.h>


static const uint8_t PT2257_ADDRESS	= 0x44;
static const uint8_t PT2257_MIN_ATTN = 0;
static const uint8_t PT2257_MAX_ATTN = 79;
static const uint16_t ANALOG_DEFAULT_MIN = 0;
static const uint16_t ANALOG_DEFAULT_MAX = 1023;


PT2257::PT2257(uint8_t aAnalogInputPin) :
		PT2257(aAnalogInputPin, PIN_NULL) {
}


PT2257::PT2257(uint8_t aAnalogInputPin, int8_t aMutePin) :
        PT2257(aAnalogInputPin, aMutePin, PT2257_MIN_ATTN, PT2257_MAX_ATTN) {
}


PT2257::PT2257(uint8_t aAnalogInputPin, int8_t aMutePin,
               uint8_t aMinAttn, uint8_t aMaxAttn) :

               analog_pin(aAnalogInputPin),
               analog_min(ANALOG_DEFAULT_MAX),
               analog_max(ANALOG_DEFAULT_MIN),
               attenuation_min(aMinAttn),
               attenuation_max(aMaxAttn) {
    
    // this chip needs some time to init, block all !
    delay(200);

	pinMode(analog_pin, INPUT);
    Wire.begin();

    // config mute button if any
    mute_button = NULL;
    if(aMutePin != PIN_NULL) {
        mute_button = new Button(aMutePin, -1, this);
    }

    // set max volume
    Wire.beginTransmission(PT2257_ADDRESS);
    Wire.write(0b11100000);
    Wire.endTransmission();

	dprintln(F("PT2257 initialized."));
}


void PT2257::update(int aAnalogValue) {

	uint8_t db_tens, db_units;
    int db_attenuation;

    // update analog floor / ceiling values
    if(analog_min > aAnalogValue) {
        analog_min = aAnalogValue;
        // dprint(F("PT2257: new analog min: "));
        // dprintln(analog_min);
    }
    if(analog_max < aAnalogValue) {
        analog_max = aAnalogValue;
        // dprint(F("PT2257: new analog max: "));
        // dprintln(analog_max);
    }
    
    // attenuation goes from -79 to 0 db
    db_attenuation = map(aAnalogValue, analog_min, analog_max, attenuation_min, attenuation_max);
    
    // extract tens and units
    db_tens = (db_attenuation / 10) & 0b0000111; // force limit to 3 bits
    db_units = (db_attenuation % 10) & 0b0001111; // force limit to 4 bits
    
#ifdef _DEBUG
    static int prev_attn = -1;
    if(prev_attn != db_attenuation) {    
        prev_attn = db_attenuation;
        // dprint(F("PT2257 analog read: "));
        // dprintln(aAnalogValue);
        dprint(F("PT2257 attenuation: "));
        dprint(db_attenuation);
        dprintln(F("db"));
        // dprint(db_tens);
        // dprint(F(" + "));
        // dprintln(db_units);
        // dprint(F("PT2257 send tens: 0b"));
        // dprintbinln(0b11100000 | db_tens);
        // dprint(F("PT2257 send units: 0b"));
        // dprintbinln(0b11010000 | db_units);
    }
#endif 

    Wire.beginTransmission(PT2257_ADDRESS);

    // write tens on both channels
    Wire.write(0b11100000 | db_tens);
    
    // write units on both channels
    Wire.write(0b11010000 | db_units);

    Wire.endTransmission();
}


void PT2257::disable() {
    dprintln(F("PT2257 disable"));

    Wire.beginTransmission(PT2257_ADDRESS);
    Wire.write(0b11111111);
    Wire.endTransmission();
}


void PT2257::mute(bool aMute) {
    dprint(F("PT2257 mute: "));
    dprintln(aMute ? F("ON") : F("OFF"));

    Wire.beginTransmission(PT2257_ADDRESS);
    Wire.write(aMute ? 0b01111001 : 0b01111000);
    Wire.endTransmission();
}


void PT2257::scan() {
	update(analogRead(analog_pin));
    if(mute_button != NULL) {
        mute_button->scan();
    }
}


void PT2257::onButtonEvent(uint8_t aPin, EButtonScanResult aResult) {
    static bool _mute = false;
    if(aResult == EButtonClick) {
        _mute = !_mute;
        mute(_mute);
    }
}

