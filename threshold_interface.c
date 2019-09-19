// todo: don't use Arduino libraries, they are very slow.
// todo: use registers directly.
#include <Arduino.h>
#include "threshold_interface.h"
#include "pins.h"

#define max_threshold 255

volatile uint32_t threshold_rise_time = 0;
volatile uint32_t threshold_period = 1;
volatile uint32_t threshold_pulse_width = 0;

uint8_t get_threshold() {
    return threshold_pulse_width * max_threshold / threshold_period;
}

void threshold_isr() {
    const uint32_t t = micros();
    if (digitalRead(THRESHOLD)) {
        threshold_period = t - threshold_rise_time;
        threshold_rise_time = t;
    } else {
        threshold_pulse_width = micros() - threshold_rise_time;
    }
}

void threshold_interface_setup() {
    attachInterrupt(digitalPinToInterrupt(THRESHOLD), threshold_isr, CHANGE);
}
