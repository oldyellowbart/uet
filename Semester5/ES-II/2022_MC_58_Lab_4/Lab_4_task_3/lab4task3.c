/*----------------------------------------------------------------------- 
Lab_04_task_1 task recreate using library & Example
-------------------------------------------------------------------------
Task 4.1: Use WTIMER2 (32-bit, on-shot timer) to flash on-board green LEDs
of TIVA C Series LaunchPad at 1 Hz frequency.
-------------------------------------------------------------------------
Date			:	09-NOV-2024.
Author		: Haseeb-ul-hassan 
Github		:	Oldyellowbart 
Compiler	: V6
IDE used	: Keil uVision 5.38a
CMSIS			:	5.9
------------------------------------------------------------------------*/
//______________________________________________________________________//
#include<TM4C123.h>

#define PIN3 0x3

void GPIO_PORTF_Init(void);
void WTIMER2_oneshot();
unsigned volatile long i;
unsigned volatile long j;

int main(){
	// Initialize PortF pin 3 as digital out
	GPIO_PORTF_Init();
	WTIMER2_oneshot();
	
	while(1){
		GPIOF->DATA ^= 1<<3;
		while((WTIMER2->RIS & 1<<0)==0);
		WTIMER2->ICR = 1<<0;
		
}
}

void GPIO_PORTF_Init(void){
	/* function to initialize the GPIO PortF pin 3
	by manipulating the configuration registers */
	
	// Step 1: Clock enable on PortF
	SYSCTL->RCGC2 |= 1<<5;
	
	//RCGC_GPIO_R |= GPIO_PORTF_CLOCK_EN;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: APB is selected for PortF by selecting
	// 0x40025000 as Base Address in DATA section
	
	// Step 3: Disable alternate functionality on PortF
	GPIOF->AFSEL &= ~(1<<PIN3);
	
	// Step 4: Enable digital pin functionaliy on PortF pin 3
	GPIOF->DEN |= 1<<PIN3; // Digital enable for PF3
	
	// Step 5: Set PortF pin 3 as an output pin
	GPIOF->DIR |= 1<<PIN3; // PF3 as output
}

void WTIMER2_oneshot(){
	
	// This function generations mDelay msecond delay
	// Note: This function can generate 4 msec delay (maximum)
	
	// Step 1: Enable Timer Clock on WTIMER2
	SYSCTL->RCGCWTIMER |= 1<<2;		// b 0000 0100
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: Ensure Timer is disabled before making any changes
	WTIMER2->CTL = 0x00;					// TAEN = 0, i.e., WTIMER2A is disablled
	
	// Step 3: Select Mode of Operation of WTIMER2 (Split/cancatenated/RTC)
	WTIMER2->CFG |= 0<<0;					// WTIMER2 is used as a 32-bit (split) timer, i.e., WTIMER2A
	WTIMER2->TAMR = 2<<0; 				// TAMR = 1 (1-shot), TACDIR = 0 (count-down)
	
	// Step 4: Load counter start value in Interval Load Register
	WTIMER2->TAILR = 8000000 - 1;		// for 16 MHz, 8000 will generate 1 mSec delay

	// Step 5: Interrupt configurations
	//	WTIMER2->IMR = 0x00;					// No interrupt enabled (RO Register)
	WTIMER2->ICR = 1<<0;					// Clear timer status flag (TATORIS, TATOMIS)
	
	// Step 6: Enable the Timer and start counting
	WTIMER2->CTL |= 1<<0;					// TAEN = 1
	
	// Step 7: Poll TATORIS (Time-Out Raw Interrupt Status) bit to check WTIMER2A timeout
	//while((WTIMER2->RIS & 1<<0)==0); // Wait for timeout flag to set
	
	// After time-out, timer stops automatically in one-shot mode
	//WTIMER2->ICR = 1<<0;					// Clear timer status flag (TATORIS, TATOMIS)
}
