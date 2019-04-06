#include "MK64F12.h"
#include "adc.h"
#include "ftm.h"
#include "pwm.h"
#include "gpio.h"
#include "dac.h"
#include "interrupts.h"

#define SOUND_FREQUENCY 100
#define REFERENCE 3.3
#define AVERAGE_COUNT 10000

// Threshold of open / close action.
// These are changed during the calibration phase.
double THRESHOLD_1_LOW = 1.1;
double THRESHOLD_1_HIGH = 1.1;
double THRESHOLD_2_LOW = 2.2;
double THRESHOLD_2_HIGH = 2.2;

// Control state of hand open/close.
int P_HAND_CLOSE = 0;
int P_HAND_OPEN = 0;

// 0.0025 max rotation (Open), 0.0005 min rotation (Close)
#define DUTY_MIN 0.00125
#define DUTY_MAX 0.0025

// Variable duty
double DUTY = 0.0005;

double ADC_MAX = REFERENCE;
double ADC_MIN = 0.0;

// The min and max output voltages for the DAC.
#define DAC_MAX 0.04
#define DAC_MIN 0

// The voltage to output to the DAC
double DAC_VALUE = 0;

int HIGH = 0;

/**
 * Generates a square wave using interrupts.
 */ 
void FTM2_IRQHandler() {
	if (HIGH) {
		dac_out(0);
	} else {
		dac_out(DAC_VALUE);
	}

	// Flip the bit from 1 -> 0 or 0 -> 1
	HIGH ^= 1;

	// set the timer overflow flag to 0 to clear interrupt.
	FTM2_SC &= ~FTM_SC_TOF_MASK;
}

/**
 * Converts a value read into the ADC into an output value for the DAC.
 * 
 * @param value The value read into the ADC.
 * @return The DAC value to output.
 */ 
double in_adc_to_dac(double value) {
	// Clamp input value between max and min.
	if (value > ADC_MAX) {
		value = ADC_MAX;
	}
	else if (value < ADC_MIN) {
		value = ADC_MIN;
	}

	double conversion = (value - ADC_MIN) / (ADC_MAX - ADC_MIN);
	return DAC_MAX * conversion;
}

/**
 * Determines which hand state we are currently in.
 */ 
void check_hand_state(double value) {
	if (value <= THRESHOLD_1_LOW) {
		P_HAND_CLOSE = 0;
		P_HAND_OPEN = 1;
	} else if(value > THRESHOLD_1_HIGH && value <= THRESHOLD_2_LOW) {
		P_HAND_CLOSE = 0;
		P_HAND_OPEN = 0;
	} else if(value > THRESHOLD_2_HIGH) {
		P_HAND_CLOSE = 1;
		P_HAND_OPEN = 0;
	}
}

/**
 * Converts a value from one range to another.
 * 
 * Example
 * ```
 * convert_range(1.5, 1, 2, 4, 6) // returns 5
 * ```
 * 
 * @param value The initial value.
 * @param in_min The minimum value for the initial range.
 * @param in_max The maximum value for the initial range.
 * @param out_min The minimum value for the output range. 
 * @param out_max The maximum value for the output range. 
 * @return The converted value.
 */ 
double convert_range(double value, double in_min, double in_max, double out_min, double out_max) {
	return ((value - in_min) / (in_max - in_min)) * (out_max - out_min) + out_min;
}

/**
 * Completes the calibration phase of the prosthetic arm. We first take severale measurements when 
 * the forearm in relaxed and then more measurements when the forearm is active. This allows us to
 * to determine the best range for operation.  
 */ 
void calibration_state() {
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

	THRESHOLD_1_LOW = convert_range(THRESHOLD_1_LOW, 0, REFERENCE, ADC_MIN, ADC_MAX);
	THRESHOLD_1_HIGH = convert_range(THRESHOLD_1_HIGH, 0, REFERENCE, ADC_MIN, ADC_MAX);
	THRESHOLD_2_LOW = convert_range(THRESHOLD_2_LOW, 0, REFERENCE, ADC_MIN, ADC_MAX);
	THRESHOLD_2_HIGH = convert_range(THRESHOLD_2_HIGH, 0, REFERENCE, ADC_MIN, ADC_MAX);
}

/**
 * The main method for the prosthetic arm.
 */ 
int main() {
	// Setup pins and registers
	adc_init();
	pwm_init();
	ftm_init();
	gpio_init();
	dac_init();

	calibration_state();

	// Start sound after calibration state
	// Divide by 2 since each period has one low phase and one high phase.
	double delay = 1.0 / SOUND_FREQUENCY / 2;
	ftm2_init(delay);

	while(1) {
		double value = adc_read();
		DAC_VALUE = in_adc_to_dac(value);
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

	return 0;
}
