/*
 * User event => callback event
 * ----------------------------
 * Press button => EButtonDown
 * Release button (before longpress time) => EButtonUp then EButtonClick
 * Keep button pressed for lonpress time => EButtonLongpress
 * Release button (after longpress time) => EButtonUnlongpress
 *
 */
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>
#include <Arduino.h>

#define PIN_NULL    (-1)

typedef enum EButtonScanResult {
    EButtonUp = 0,      // off, idle state
    EButtonDown,        // button is pressed
    EButtonClick,       // down then up events happened < longpress time
    EButtonLongpress,   // button help down for > longpress time
    EButtonUnlongpress  // button up from longpress
} EButtonScanResult;

typedef void (*button_cb_t)(uint8_t, EButtonScanResult);

class ButtonListener {
public:
    virtual void onButtonEvent(uint8_t aPin, EButtonScanResult aResult) = 0;
};

class Button {
private:
    int8_t              prev_pin;
    uint8_t             pin;
    bool                longpress;
    uint32_t            longpressTS;
    uint16_t            longpressDelay;
    ButtonListener *    listener;
    void                onButtonReleased();
    void                onButtonPressed();
public:
    Button(uint8_t aPin, uint16_t aLongpressDelay, ButtonListener * aListener);
    void scan();
};

#endif // __BUTTON_H__

