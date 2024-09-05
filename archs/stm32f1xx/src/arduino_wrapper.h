#ifndef _ARDUIN_WRAPPER_H_
#define _ARDUIN_WRAPPER_H_

#include "arch.h"

#define PIN_A0 0
#define PIN_A1 1
#define PIN_A2 2
#define PIN_A3 3
#define PIN_A4 4
#define PIN_A5 5
#define PIN_A6 6
#define PIN_A7 7

#define PIN_A8 16
#define PIN_A9 17

#define PA0 PIN_A0
#define PA1 PIN_A1
#define PA2 PIN_A2
#define PA3 PIN_A3
#define PA4 PIN_A4
#define PA5 PIN_A5
#define PA6 PIN_A6
#define PA7 PIN_A7
#define PA8 8
#define PA9 9
#define PA10 10
#define PA11 11
#define PA12 12
#define PA13 13
#define PA14 14
#define PA15 15
#define PB0 PIN_A8
#define PB1 PIN_A9
#define PB2 18
#define PB3 19
#define PB4 20
#define PB5 21
#define PB6 22
#define PB7 23
#define PB8 24
#define PB9 25
#define PB10 26
#define PB11 27
#define PB12 28
#define PB13 29
#define PB14 30
#define PB15 31
#define PC13 32
#define PC14 33
#define PC15 34
#define PD0 35
#define PD1 36

#define OUTPUT 0
#define INTPUT 1
#define INPUT_PULLUP 2
#define INPUT_PULLDOWN 3

#define LOW 0
#define HIGH 1

void pinMode(uint16_t pinNum, uint8_t state);
void digitalWrite(uint16_t pinNum, uint8_t state);
bool digitalRead(uint16_t pinNum);

#endif