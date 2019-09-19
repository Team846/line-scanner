#include "result_interface.h"

extern "C" {
#include "exposure_interface.h"
#include "threshold_interface.h"
#include "pins.h"
}

void setup(void) {
    pinMode(SI, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(AO, INPUT);

    si_lo;
    clk_lo;

    pinMode(EXPOSURE, INPUT);
    pinMode(THRESHOLD, INPUT);
    pinMode(RESULT, OUTPUT);

    exposure_interface_setup();
    threshold_interface_setup();
    result_interface_setup();

    Serial.begin(115200);
}

void loop(void) {

    // read settings from application
    // led_hi;
    const uint16_t exposure = get_exposure();
    const uint8_t threshold = get_threshold();
    Serial.write((uint8_t) 0);
    // led_lo;

    const uint32_t start = micros();
    clear_sensor_buffer();

    // wait for exposure
    while (micros() - start < exposure);
    led_lo;

    const uint8_t result = process_sensor_data(threshold);
    set_result(result);
    Serial.write(result + 1);
    Serial.write((exposure + 1) >> 8);
    Serial.write((exposure + 1) >> 0);
    Serial.write(threshold + 1);
}

void clear_sensor_buffer() {
    prime_sensor;
    for (uint8_t i = 0; i < resolution; i++) {
        clk_hi;
        clk_lo;
    }
}

const uint8_t optical_compensation[] = {6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 13, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 29, 29, 30, 31, 31, 31, 30, 31, 31, 31, 27, 31, 31, 31, 31, 30, 30, 30, 30, 31, 30, 29, 29, 30, 29, 30, 30, 29, 29, 30, 30, 30, 29, 29, 29, 29, 29, 29, 28, 28, 28, 27, 27, 27, 26, 26, 26, 26, 25, 25, 25, 25, 25, 25, 25, 24, 25, 25, 25, 24, 24, 24, 24, 23, 22, 23, 22, 22, 22, 21, 20, 20, 18, 17, 16, 15, 14, 12, 11, 1, 183, 223, 29};
// const uint8_t optical_compensation[] = {99, 108, 111, 114, 119, 122, 124, 131, 132, 135, 137, 137, 138, 137, 136, 136, 132, 131, 128, 124, 120, 118, 113, 111, 104, 102, 97, 91, 87, 84, 80, 78, 75, 73, 70, 68, 67, 65, 64, 64, 62, 62, 61, 59, 58, 57, 56, 55, 56, 56, 56, 56, 56, 57, 59, 61, 64, 66, 66, 60, 69, 70, 71, 71, 70, 70, 70, 69, 69, 68, 67, 66, 66, 66, 66, 68, 67, 67, 67, 68, 67, 67, 65, 67, 69, 70, 72, 73, 73, 75, 75, 78, 79, 79, 76, 75, 74, 75, 74, 74, 74, 78, 79, 82, 80, 87, 87, 89, 90, 90, 90, 86, 82, 76, 78, 77, 73, 72, 69, 65, 61, 58, 59, 59, 58, 58, 58, 56, 1, 183, 223, 29};

uint8_t process_sensor_data(const uint8_t threshold) {
    prime_sensor;

    uint32_t numerator = 0;
    uint16_t denominator = 0;

    for (uint16_t i = 0; i < resolution; i++) {
        const uint8_t compensation = optical_compensation[i];
        const uint8_t raw_pixel = ADC_8BIT(AO);

        uint8_t compensated_pixel = 0;
        if (raw_pixel > compensation) compensated_pixel = raw_pixel - compensation;

        if (compensated_pixel > threshold) {
            numerator += i * compensated_pixel;
            denominator += compensated_pixel;
        }

        if (raw_pixel == 255) {
            led_hi;
            Serial.write(255);
        } else {
            Serial.write(raw_pixel + 1);
        }

        clk_hi;
        clk_lo;
    }

    if (denominator == 0) return 0;
    else return (numerator / denominator) + 1;
}
