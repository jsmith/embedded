//#include "fsl_device_registers.h"
//#include "dac.h"
//#include "mario.h"
//#include "adc.h"
//#include "pwm.h"
//
//// Threshold of open / close action.
//#define THRESHOLD_1_LOW 1.1
//#define THRESHOLD_1_HIGH 1.1
//#define THRESHOLD_2_LOW 2.2
//#define THRESHOLD_2_HIGH 2.2
//
//#define DUTY_MIN 0.0005
//#define DUTY_MAX 0.0025
//#define DUTY_STEP 0.00001
//
//// ADC max & min
//#define ADC_MAX 3.3
//
//#define DAC_MAX 0.01
//#define DAC_MIN 0
//
//double DAC_FACTOR = (4095.0 / 3.3);
//
//// https://community.nxp.com/thread/430040
//
//double ADC_VALUE = 0;
//int HAND_CLOSE = 0;
//int HAND_OPEN = 0;
//
//int HIGH = 0;
//int NOTE = 0;
//int COUNT = 0;
//double PERIOD = 0.0001; // This default value could cause issues
//short DAC_VALUE = 50;
//double TIME_PASSED = 0;
//int CLOCK_FREQUENCY = 20960000;
//double SLOW_DOWN = 1.5;
//
///*
// * Interrupt callback triggered from TOF flag in FTM2
// *
// * This has to have a very specific name, thats how the
// * board relates this function to the interrupts. It has
// * to be in the format (MODULE_NAME)_IRQHandler.
// *
// * EX. PORTB_IRQHandler, ADC0_IRQHandler, etc.
// *
// */
//void FTM2_IRQHandler( void ) {
//	// Handle the interrupt anyway that you like here
//	if (COUNT == 0) {
//		double noteDuration = SLOW_DOWN / MARIO_BEATS[NOTE];
//		if (MARIO_MELODY[NOTE] != 0) {
//			PERIOD = SLOW_DOWN / MARIO_MELODY[NOTE];
//			DAC_VALUE = 5;
//		} else {
//			DAC_VALUE = 0;
//		}
//		COUNT = noteDuration / PERIOD;
//		NOTE = (NOTE + 1) % MARIO_SIZE;
//	} else {
//		if (HIGH) {
//			dac_out(0);
//		} else {
//			dac_out(DAC_VALUE);
//		}
//		HIGH ^= 1;
//		COUNT--;
//	}
//
//	/*
//	 * For any of the modules you have to clear whatever flag
//	 * made the interrupt happen, if you don't you're execution
//	 * will just end up right back here
//	 */
//	FTM2_SC &= ~FTM_SC_TOF_MASK; // set the timer overflow flag to 0 to clear interrupt.
//
//	// THIS needs to go here to work...
//	// IDK why hahahhahaha
//	FTM2_MOD = 0xFFFF & ((int)(PERIOD * CLOCK_FREQUENCY));
//}
//
//void FTM2_init() {
//	/*
//	 * The two important functions here are:
//	 * NVIC_DisableIRQ( IRQn ) and NVIC_EnableIRQ( IRQn )
//	 *
//	 * Disable will disable any interrupts for that function if they are
//	 * enabled.
//	 * Enabled will enable any interrupts for that function if the apropriate
//	 * flags have been set.
//	 */
//	NVIC_DisableIRQ( FTM2_IRQn ); // Disable interrupts for FTM2 if they are currently enabled
//
//	// NO CLOCK IS SELECTED FOR FTM2_SC_CLKS SO THE WAVE WILL NOT START UNTIL
//	// THE buzz() FUNCTION IS CALLED UPON
//
//	SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK; // Enable the FTM2 Clock
//
//	FTM2_MODE |= FTM_MODE_WPDIS_MASK; // Turn Write Protection Off
//
//	FTM2_SC |= (0b000 << 0); // selects pre-scale factor of 1 (default)
//
//	/*
//	 * This is where we tell the FTM2 module that we want the TOF flag to
//	 * generate an interrupt
//	 */
//	FTM2_SC |= (0b1 << 6 ); // Enable timer overflow interrupt
//
//	FTM2_CNTIN = 0x00; // Initial value of counter = 0
//
//	/*
//	 * The TOF flag will be set whenever the FTM2_CNT register reaches the MOD value
//	 * that means that using the system clock (20.96 Mhz) with a scaling factor of
//	 * 1 this will be triggered 3.13ms, for greater granularity decrease the MOD value
//	 *
//	 * Called every 1 / 20.96mhz * 0xFFFF = 3.12666985 ms
//	 */
//	FTM2_MOD = 0xFFFF;
//
//	/*
//	 * Set MSB=1 and ELSnB=1
//	 * This is output compare and will toggle the channel value each time the
//	 * counter reaches the value specified in FTM0_C0V (doesn't really matter
//	 * in this case sense we just want to know when there is a overflow)
//	 */
//	FTM2_C0SC |= 0x28;
//
//	/*
//	 * This enables the interrupts, now when the TOF flag is set execution will
//	 * be switched to FTM2_IRQHandler()
//	 */
//	NVIC_EnableIRQ( FTM2_IRQn );
//
//	FTM2_SC |= (0b1000); // selects the system clock, the counter will now start
//}
//
//int IN_DUTY = 0;
//double PWM_PERIOD = 0.02;
//double DUTY = 0.002;
//double PWM_DELAY = 0.002; // the default will be overwritten
//
//double in_read_adc() {
//	PWM_DELAY = DUTY;
//	ADC_VALUE = adc_read();
//
//	if (ADC_VALUE > ADC_MAX) {
//		ADC_VALUE = ADC_MAX;
//	}
//	else if (ADC_VALUE < 0) {
//		ADC_VALUE = 0;
//	}
//}
//
//double in_adc_to_dac() {
//	double conversion = ADC_VALUE / ADC_MAX;
//	DAC_VALUE = DAC_MAX * conversion * DAC_FACTOR;
//}
//
//void in_check_hand_state() {
//	if (ADC_VALUE <= THRESHOLD_1_LOW) {
//		HAND_CLOSE = 0;
//		HAND_OPEN = 1;
//	}
//	else if (ADC_VALUE >= THRESHOLD_2_HIGH) {
//		HAND_CLOSE = 1;
//		HAND_OPEN = 0;
//	}
//	else {
//		HAND_CLOSE = 0;
//		HAND_OPEN = 0;
//	}
//}
//
//void FTM1_IRQHandler () {
//	if (IN_DUTY) {
//		pwm_clear_output();
//		PWM_DELAY = PWM_PERIOD - DUTY;
//	} else {
//		in_read_adc();
//		in_adc_to_dac();
//		in_check_hand_state();
//
//		if (HAND_CLOSE == 1 &&  HAND_OPEN == 0) {
//			DUTY -= DUTY_STEP;
//			if (DUTY < DUTY_MIN) {
//				DUTY = DUTY_MIN;
//			}
//		} else if (HAND_CLOSE == 0 &&  HAND_OPEN == 1){
//			DUTY += DUTY_STEP;
//			if (DUTY > DUTY_MAX) {
//				DUTY = DUTY_MAX;
//			}
//		}
//
//		pwm_set_output();
//	}
//
//	IN_DUTY ^= 1;
//	FTM1_SC &= ~FTM_SC_TOF_MASK;
//	FTM1_MOD = 0xFFFF & ((int)(PWM_DELAY * CLOCK_FREQUENCY));
//}
//
//void FTM1_init() {
//	NVIC_DisableIRQ( FTM1_IRQn );
//	SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;
//	FTM1_MODE |= FTM_MODE_WPDIS_MASK;
//	FTM1_SC |= (0b000 << 0);
//	FTM1_SC |= (0b1 << 6 );
//	FTM1_CNTIN = 0x00;
//	FTM1_MOD = 0xFFFF;
//	FTM1_C0SC |= 0x28;
//	NVIC_EnableIRQ( FTM1_IRQn );
//	FTM1_SC |= (0b1000);
//}
//
//int main_interrupt() {
//	dac_init();
//	adc_init();
//	FTM2_init();
//	FTM1_init();
//	pwm_init();
//
//    // This for loop should be replaced. By default this loop allows a single
//	// stepping.
//    for (;;) {}
//
//    return 0;
//}
