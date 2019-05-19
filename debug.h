#include <Arduino.h>
#define _DEBUG
#ifdef _DEBUG
#define     dprintinit(x)           Serial.begin(x)
#define     dprint(x)               Serial.print(x)
#define     dprintln(x)             Serial.println(x)
#define     dprintbin(x)            Serial.print(x, BIN)
#define     dprintbinln(x)          Serial.println(x, BIN)
#else
#define     dprintinit(x)
#define     dprint(x)
#define     dprintln(x)
#endif
