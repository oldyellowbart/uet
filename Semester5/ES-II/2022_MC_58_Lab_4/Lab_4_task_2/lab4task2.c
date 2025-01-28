/*----------------------------------------------------------------------- 
Lab_04_task_1 task recreate using library & Example
-------------------------------------------------------------------------
Task 4.2: Use timer3A (16-bit, periodic timer, count-up timer ) to flash
on-board green LEDs of TIVA C Series LaunchPad at 1 Hz frequency.
-------------------------------------------------------------------------
Date			:	09-NOV-2024.
Author		: Haseeb-ul-hassan 
Github		:	Oldyellowbart 
Compiler	: Arm V6
IDE used	: Keil uVision 5.38a
CMSIS			:	5.9
------------------------------------------------------------------------*/
//______________________________________________________________________//
#include<TM4C123.h>

#define PIN3 0x3

void GPIO_PORTF_Init(void);
void TIMER3_periodic();
unsigned volatile long i;
unsigned volatile long j;
int k;
int main(){
	// Initialize PortF pin 3 as digital out
	GPIO_PORTF_Init();
	TIMER3_periodic();
	
	while(1){

		GPIOF->DATA ^= 1<<3;
		k=TIMER3->TAMR;
		while((TIMER3->RIS & 1<<0)==0);
		TIMER3->ICR = 1<<0;
		
		
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


void TIMER3_periodic(){
	
	// This function generations mDelay msecond delay
	// Note: This function can generate 4 msec delay (maximum)
	
	// Step 1: Enable Timer Clock on TIMER3
	SYSCTL->RCGCTIMER |= 1<<3;		// b 0000 1000
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: Ensure Timer is disabled before making any changes
	TIMER3->CTL = 0x00;					// TAEN = 0, i.e., TIMER3A is disablled
	
	// Step 3: Select Mode of Operation of TIMER3 (Split/cancatenated/RTC)
	TIMER3->CFG |= 0<<2;					// TIMER3 is used as a 16-bit (split) timer, i.e., TIMER3A
	TIMER3->TAMR = (1<<1 | 0<<2 | 1<<4); 				// TAMR = 1 (1-shot) TACDIR = 1 (count-up)
	// Step 4: Load counter start value in Interval Load Register
	TIMER3->TAILR = 8000000 - 1;		// for 16 MHz, 80,00,000 will generate 1 Sec delay
	// Step 5: Interrupt configurations
	//	TIMER3->IMR = 0x00;					// No interrupt enabled (RO Register)
	TIMER3->ICR = 1<<0;					// Clear timer status flag (TATORIS, TATOMIS)
	
	// Step 6: Enable the Timer and start counting
	TIMER3->CTL |= 1<<0;					// TAEN = 1
	
	// Step 7: Poll TATORIS (Time-Out Raw Interrupt Status) bit to check TIMER3A timeout
	//while((TIMER3->RIS & 1<<0)==0); // Wait for timeout flag to set
	
	// After time-out, timer stops automatically in one-shot mode
	//TIMER3->ICR = 1<<0;					// Clear timer status flag (TATORIS, TATOMIS)
}
