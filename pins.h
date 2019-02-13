#define ADC_8BIT(PIN) analogRead(PIN) >> 2

// DEBUGGING
#define LED 11
#define led_hi digitalWrite(LED, HIGH)
#define led_lo digitalWrite(LED, LOW)

// LINE SCANNER INPUTS
#define resolution 128

#define AO  21
#define SI  20
#define CLK 19

#define clk_lo digitalWrite(CLK, LOW)
#define clk_hi digitalWrite(CLK, HIGH)
#define si_lo  digitalWrite(SI,  LOW)
#define si_hi  digitalWrite(SI,  HIGH)

#define prime_sensor si_hi; clk_hi; si_lo; clk_lo;

// APPLICATION INTERFACE
#define EXPOSURE  5
#define THRESHOLD 6
#define RESULT    7
