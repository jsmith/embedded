#include "MK64F12.h"
#include "console.h"
#include "adc.h"
#include "ftm.h"
#include "pwm.h"
#include "gpio.h"
#include "dac.h"
#include "converter.h"
#include "interrupts.h"

#define MAX 50
#define AVERAGE_COUNT 10000

// Threshold of open / close action.
double THRESHOLD_1_LOW = 1.1; // 0.7
double THRESHOLD_1_HIGH = 1.1; // 1.3
double THRESHOLD_2_LOW = 2.2; // 2.0
double THRESHOLD_2_HIGH = 2.2; // 2.6

// Control state of hand open/close.
int P_HAND_CLOSE = 0;
int P_HAND_OPEN = 0;

// 0.0025 max rotation (Open), 0.0005 min rotation (Close)
double DUTY_MIN = 0.00125;
double DUTY_MAX = 0.0025;

// Variable duty
double DUTY = 0.0005;

double ADC_MAX = 3.3;
double ADC_MIN = 0.0;
short DAC_VALUE = 0;

double DAC_FACTOR = (4095.0 / 3.3);
#define DAC_MAX 0.04
#define DAC_MIN 0

#define DELAY 0.005
int HIGH = 0;

void FTM2_IRQHandler() {
	if (HIGH) {
		dac_out(0);
	} else {
		dac_out(DAC_VALUE);
	}
	HIGH ^= 1;

	// set the timer overflow flag to 0 to clear interrupt.
	FTM2_SC &= ~FTM_SC_TOF_MASK;
}

double in_adc_to_dac(double value) {
	// Clamp input value between max and min.
	if (value > ADC_MAX) {
		value = ADC_MAX;
	}
	else if (value < ADC_MIN) {
		value = ADC_MIN;
	}

	double conversion = (value - ADC_MIN) / (ADC_MAX - ADC_MIN);
	return DAC_MAX * conversion * DAC_FACTOR;
}

// NOTE:  UART disabled as causes weird jitter glitch with servo.
void check_hand_state(double value) {
	if (value <= THRESHOLD_1_LOW && (P_HAND_CLOSE != 0 || P_HAND_OPEN != 1)) {
		P_HAND_CLOSE = 0;
		P_HAND_OPEN = 1;
	}
	else if(value > THRESHOLD_1_HIGH && value <= THRESHOLD_2_LOW && (P_HAND_CLOSE != 0 || P_HAND_OPEN != 0)) {
		P_HAND_CLOSE = 0;
		P_HAND_OPEN = 0;
	}
	else if(value > THRESHOLD_2_HIGH && (P_HAND_CLOSE != 1 || P_HAND_OPEN != 0)) {
		P_HAND_CLOSE = 1;
		P_HAND_OPEN = 0;
	}
}

double convert_range(double value, double in_min, double in_max, double out_min, double out_max) {
	return ((value - in_min) / (in_max - in_min)) * (out_max - out_min) + out_min;
}

void calibration_state() {
	// double values[AVERAGE_COUNT];

	// Test button click
	blue(1); // REady for open
	wait_button_click();

	red(1); // REady for close
	double sum = 0;
	for (int i = 0; i < AVERAGE_COUNT; i++) {
		sum += adc_read();
	}
	ADC_MIN = sum / AVERAGE_COUNT;

	blue(1);
	wait_button_click();
	red(1);

	sum = 0;
	for (int i = 0; i < AVERAGE_COUNT; i++) {
		sum += adc_read();
	}
	ADC_MAX = sum / AVERAGE_COUNT;

	green(1);

	THRESHOLD_1_LOW = convert_range(THRESHOLD_1_LOW, 0, 3.3, ADC_MIN, ADC_MAX);
	THRESHOLD_1_HIGH = convert_range(THRESHOLD_1_HIGH, 0, 3.3, ADC_MIN, ADC_MAX);
	THRESHOLD_2_LOW = convert_range(THRESHOLD_2_LOW, 0, 3.3, ADC_MIN, ADC_MAX);
	THRESHOLD_2_HIGH = convert_range(THRESHOLD_2_HIGH, 0, 3.3, ADC_MIN, ADC_MAX);
}

int main() {
	// Setup pins and registers
	uart_init();
	adc_init();
	pwm_init();
	ftm_init();
	gpio_init();
	dac_init();

	calibration_state();

	// Start sound after calibration state
	FTM2_init(DELAY);

	while(1) {
		double value = adc_read();
		DAC_VALUE = in_adc_to_dac(value);

		// dtoa(value, formatted, 2);
		// dac_out(value * 1241);
		check_hand_state(value);

		if(P_HAND_CLOSE == 1 &&  P_HAND_OPEN == 0) {
			DUTY -= 0.00001;
			if (DUTY < DUTY_MIN) {
				DUTY = DUTY_MIN;
			}
		}
		else if(P_HAND_CLOSE == 0 &&  P_HAND_OPEN == 1){
			DUTY += 0.00001;
			if (DUTY > DUTY_MAX) {
				DUTY = DUTY_MAX;
			}
		}

		pwm_clear_output();
		ftm_delay(0.02 - DUTY);

		pwm_set_output();
		ftm_delay(DUTY);
	}

	while (1);


	return 0;
}
