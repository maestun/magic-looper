#include <Arduino.h>
#include "optoloop.h"
#include "pt2257.h"
#include "debug.h"

#define     LOOP_A_PIN_LED_TEMP    	(2)
#define     LOOP_A_PIN_LED_TOGGLE   (3)
#define 	LOOP_A_PIN_OCTOCOUPLER	(4)
#define 	LOOP_A_PIN_BYPASS		(5)
#define     LOOP_A_PIN_RELAY        (13)

#define     LOOP_B_PIN_LED_TEMP    	(9)
#define     LOOP_B_PIN_LED_TOGGLE   (8)
#define 	LOOP_B_PIN_OCTOCOUPLER	(10)
#define 	LOOP_B_PIN_BYPASS		(6)
#define     LOOP_B_PIN_RELAY        (11)

#define     LONGPRESS_THRESHOLD_MS  (500)
#define     AUDIO_CUT_MS            (20)


Optoloop *  gLoopA;
Optoloop *  gLoopB;
PT2257 *	gVolume;



// ============================================================================
void setup() {
    dprintinit(9600);
    dprintln(F("BEGIN"));

    gLoopA = new Optoloop(0, LOOP_A_PIN_BYPASS, LOOP_A_PIN_OCTOCOUPLER, 
    					  LOOP_A_PIN_RELAY, LOOP_A_PIN_LED_TEMP, LOOP_A_PIN_LED_TOGGLE, 
    					  LONGPRESS_THRESHOLD_MS, AUDIO_CUT_MS);
    gLoopB = new Optoloop(1, LOOP_B_PIN_BYPASS, LOOP_B_PIN_OCTOCOUPLER, 
    					  LOOP_B_PIN_RELAY, LOOP_B_PIN_LED_TEMP, LOOP_B_PIN_LED_TOGGLE, 
    					  LONGPRESS_THRESHOLD_MS, AUDIO_CUT_MS);
    gVolume = new PT2257(A0);
}


void loop() {
    gLoopA->scan();
    gLoopB->scan();
    gVolume->scan();
    delay(1);
}
