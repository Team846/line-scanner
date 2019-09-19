// todo: don't use Arduino libraries, they are very slow.
// todo: use registers directly.
#include <Arduino.h>
#include "exposure_interface.h"
#include "pins.h"

#define max_exposure 50000

volatile uint32_t exposure_rise_time = 0;
volatile uint32_t exposure_period = 1;
volatile uint32_t exposure_pulse_width = 0;

uint16_t get_exposure() {
    return exposure_pulse_width * max_exposure / exposure_period;
}

void exposure_isr() {
    const uint32_t t = micros();
    if (digitalRead(EXPOSURE)) {
        exposure_period = t - exposure_rise_time;
        exposure_rise_time = t;
    } else {
        exposure_pulse_width = micros() - exposure_rise_time;
    }
}

void exposure_interface_setup() {
    attachInterrupt(digitalPinToInterrupt(EXPOSURE), exposure_isr, CHANGE);
}
