#include "fsl_device_registers.h"

void FTM0CenterAlign(void) {
//	SIM_SCGC6|=0x03000000; //enable FTM0 and FTM0 module clock
//	SIM_SCGC5=SIM_SCGC5|0x3E00; //enable port A/B/C/D/E clock
//	FTM0_CONF=0xC0; //set up BDM in 11
//	FTM0_FMS=0x00; //clear the WPEN so that WPDIS is set in FTM0_MODE register
//	FTM0_MODE|=0x05; //enable write the FTM CnV register
//	FTM0_MOD=1000;
//	FTM0_C0SC=0x28; //High_Low_High_ for center-alignment
//	FTM0_C1SC=0x28;
//	FTM0_C2SC=0x28;
//	FTM0_C3SC=0x28;
//	FTM0_COMBINE|=0x2020; //enable update the FTM_C0V/FTM0_C1V register
//	FTM0_COMBINE|=0x0202; //complementary mode for CH0&CH1, CH2&CH3
//	FTM0_COMBINE|=0x1010; // dead timer insertion enabled in complementary mode //for CH0&CH1 of
//	FTM0
//	FTM0_DEADTIME=0x00; //dead time is 0 system clock cycles
//	FTM0_C1V=500;
//	FTM0_C0V=500;
//	FTM0_C2V=500;
//	FTM0_C3V=500;
//	FTM0_SYNC|=0x01;
//	FTM0_SYNCONF|=0x80; //enable enhanced PWM synchronization
//	FTM0_CNTIN=0x00;
//	FTM0_SC=0x68; //PWM center_alignment, system clock driving, dividing by 1
}

void FTM0_ISR() {
//	static int flag;
//	if(FTM0_SC&0x80) {
//		FTM0_SC&=0x7F; //clear TOF bit
//		if(flag) {
//			FTM0_C0V=750;
//			FTM0_C1V=750;
//			FTM0_C2V=750;
//			FTM0_C3V=750;
//		} else {
//			FTM0_C0V=500;
//			FTM0_C1V=500;
//			FTM0_C2V=500;
//			FTM0_C3V=500;
//		}
//		flag ^= flag;
//		FTM0_PWMLOAD|=0x200;
//	}
}
