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
    Serial.write(exposure + 1);
    Serial.write(threshold + 1);
}

void clear_sensor_buffer() {
    prime_sensor;
    for (uint8_t i = 0; i < resolution; i++) {
        clk_hi;
        clk_lo;
    }
}

uint8_t process_sensor_data(const uint8_t threshold) {
    prime_sensor;

    uint32_t numerator = 0;
    uint16_t denominator = 0;

    for (uint16_t i = 0; i < resolution; i++) {
        const uint8_t pixel = ADC_8BIT(AO);
        if (pixel > threshold) {
            numerator += i * pixel;
            denominator += pixel;

            if (pixel == 255) led_hi;
        }

        Serial.write(pixel == 255 ? 255 : pixel + 1);
        clk_hi;
        clk_lo;
    }

    if (denominator == 0) return 0;
    else return (numerator / denominator) + 1;
}
