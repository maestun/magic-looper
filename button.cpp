/*
 * Pullup button wiring : Vcc => button => pin
 *                                      => 10k => GND
 */

#include "button.h"


Button::Button(uint8_t aPin, uint16_t aLongpressDelay, ButtonListener * aListener) {
    pinMode(aPin, INPUT);
    pin = aPin;
    longpress = false;
    longpressDelay = aLongpressDelay;
    longpressTS = 0;
    listener = aListener;
    prev_pin = PIN_NULL;
}


void Button::onButtonReleased() {

    if(pin == prev_pin) {
        // unclick
        if(longpress == false) {
            listener->onButtonEvent(pin, EButtonUp);
            listener->onButtonEvent(pin, EButtonClick);
        }
        else {
            // unlongpress
            listener->onButtonEvent(pin, EButtonUnlongpress);
            longpress = false;
        }
        prev_pin = PIN_NULL;
    }
}


// TODO: implement debounce filter !!
void Button::onButtonPressed() {

    // previous code w/ longpress detection
    if(pin == prev_pin) {
        // same pin still pressed
        if(longpress == false && (millis() - longpressTS) >= longpressDelay) {
            longpress = true;
            listener->onButtonEvent(pin, EButtonLongpress);
        }
    }
    else {
        // new button pressed
        listener->onButtonEvent(pin, EButtonDown);
        longpressTS = millis();
        prev_pin = pin;
    }
}


void Button::scan() {
    if(digitalRead(pin) == true) {
        // pressed
        onButtonPressed();
    }
    else {
        // released
        onButtonReleased();
    }
}