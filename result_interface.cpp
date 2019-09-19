// todo: don't use Arduino libraries, they are very slow.
// todo: use registers directly.
#include <Arduino.h>
#include <TimerOne.h>
#include "result_interface.h"
#include "pins.h"

#define null_pulse_width 500
#define zero_pulse_width 1000

void set_result(const uint8_t result) {
    if (result == 0) Timer1.setPeriod(null_pulse_width);
    else {
        const uint16_t line_location = result - 1;
        Timer1.setPeriod(zero_pulse_width + (line_location << 3));
    }
}

volatile uint8_t state = HIGH;

void isr_result_t() {
    state = !state;
    digitalWrite(RESULT, state);
}

void result_interface_setup() {
    Timer1.initialize(null_pulse_width);
    Timer1.attachInterrupt(isr_result_t);
    Timer1.start();
    set_result(0);
}
