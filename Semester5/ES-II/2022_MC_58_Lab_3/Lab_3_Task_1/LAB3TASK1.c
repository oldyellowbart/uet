//|_______________________________________________________________________________________________________________________________________________________|
//|Task 3.1: Modify the Example_01 program such that LED blinks at double the current frequency. You are not allowed to change the value ofRELOAD Register|
//|Hint: you need to read CURRENT register for value equal to half the RELOAD value and toggle LED (in addition to LED toggle when CURRENT turns to zero).|
//|-------------------------------------------------------------------------------------------------------------------------------------------------------|
//|Name : Haseeb-ul-hassan																												   																																		  |
//|Date : 29-SEP-2024																																																																		  |
//|Hardware: Tiva C series Launchpad Evaluation kit by Texas instruments																				  																				|	
//|IDE and Compiler : Keil microvision 5.38a																											  																											|
//|Disclaimer : This program has been written as an open source develpment program so any usage must be credited and i am not responsible for any missuse |
//|Contact : 2022mc58@student.uet.edu.pk(feel free to ask me anything about this code),Github :oldyellowbart											  											|
//|_______________________________________________________________________________________________________________________________________________________|
#include <TM4C123.h>
/*
-----------------------------------------------------------------------
This C language program toggles on-board user green LED of TIVA C Series 
LaunchPad Evaluation Kit by Texas Instrument at 2 Hz by using SysTick Timer.
-----------------------------------------------------------------------
Written by Haseeb-ul-Hassan().
Compiler and IDE used: Keil uVision 5.34 */
#include <TM4C123.h>
unsigned volatile long j;
void PF3_Digital_Output_Init();
void PF1_Digital_Output_Init();
void SysTick_Init();
int main()// User main program
			{
				PF3_Digital_Output_Init();
				PF1_Digital_Output_Init();
				SysTick_Init();
				while (1)
				{
					if(SysTick->CTRL & (1<<16))// Read SysTick Status Bit
					{		
						//GPIOF->DATA ^= 1<<3;// Toggle the green LED
						}
					if(SysTick->VAL == 0x7A1200)
						{
							GPIOF->DATA ^= 1<<3;// Toggle the green LED
					
						}
				}
			}
void PF3_Digital_Output_Init()
{
	SYSCTL->RCGCGPIO |= 1<<5; // Step 1: Clock enable on PortF
	for (j =0; j < 3 ; j++)	 // at least 3 clock cyles
	GPIOF->AFSEL &= ~(1<<3);// Step 3: Disable alternate functionality on PortF
	GPIOF->DEN |= 1<<3;    // Step 4: Enable digital pin functionaliy on PortF pin 3
	GPIOF->DIR |= 1<<3;   // Step 5: Set PortF pin 3 as an output pin
}
void PF1_Digital_Output_Init()
{
	SYSCTL->RCGCGPIO |= 1<<5; // Step 1: Clock enable on PortF
	for (j =0; j < 3 ; j++)	 // at least 3 clock cyles
	GPIOF->AFSEL &= ~(1<<1);// Step 3: Disable alternate functionality on PortF
	GPIOF->DEN |= 1<<1;    // Step 4: Enable digital pin functionaliy on PortF pin 3
	GPIOF->DIR |= 1<<1;   // Step 5: Set PortF pin 3 as an output pin
}

void SysTick_Init(void)
{
	// 1. Program the value in the STRELOAD register
	SysTick->LOAD = 16000000-1; 	// Default clock frequency
	// 2. Clear the STCURRENT register by writing to it with any value.
	SysTick->VAL = 0x00;
	// 3. Configure the STCTRL register for the required operation.
	SysTick->CTRL |= (1<<2)|(1<<0);		// SysTick enable, Main Oscillator as Clock Source

}
