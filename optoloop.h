#include "button.h"

#ifndef __OPTOLOOP_H__
#define __OPTOLOOP_H__

class Optoloop : public ButtonListener {
private:
    Button *    button;
    int8_t      id;
    uint8_t     octocoupler_pin;
    uint8_t     relay_pin;
    uint8_t     temp_led_pin;
    uint8_t     latch_led_pin;
    uint8_t     audio_cut_ms;
    bool        fx_on = false;
    bool        fx_disable = false;
    void        onButtonEvent(uint8_t aPin, EButtonScanResult aResult);
    void        toggle(bool aON, bool aTemp);
public:
    void scan();
    Optoloop(int8_t aID, uint8_t aButtonPin, uint8_t aOctocouplerPin, uint8_t aRelayPin, uint8_t aTempLEDPin, uint8_t aLatchLEDPin, uint16_t aLongpressDelayMS, uint8_t aAudioCutMS);
};

#endif // __OPTOLOOP_H__
