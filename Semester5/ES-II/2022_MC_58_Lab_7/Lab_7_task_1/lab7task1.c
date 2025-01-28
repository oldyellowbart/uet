/* Lab_07_task_1 task recreate using library & Example
-----------------------------------------------------------------------
Task 7.1: Modify the Example_01 program such that you use some other
 PWM channel and external LED and control its brightness. .
-----------------------------------------------------------------------
Written by Haseeb-ul-hassan (github-oldyellowbart) on 15-SEP-2024.
Compiler and IDE used: Keil uVision 5.31a */
//_______________________________________________________________________
// Libraries
#include "TM4C123.h"                    // Device header

unsigned volatile long j;
volatile uint32_t i;

void PLL_Init(void);
void PC4_as_M1PWM3_Init(void);
void PWM_Module1_Channel3_Init(void);
void clocks_delay(volatile uint32_t clocks);

int main(){
	PLL_Init();
	PC4_as_M1PWM3_Init();
	PWM_Module1_Channel3_Init();
	
	while(1){
		for(i=0;i<1600;i++)
		{
			PWM0->_1_CMPA = i;
			clocks_delay(1000);
		}
		clocks_delay(500000);
		for(i=1600; i>0; i--){
			PWM0->_1_CMPA = i;
			clocks_delay(1000);
		}
		clocks_delay(500000);
	}
}

void clocks_delay(volatile uint32_t clocks){
	while(clocks--);
}

// Configure the system clock to 16 MHz using PLL-------------------------------
void PLL_Init(void)// 0) Use RCC2
{ 
  SYSCTL->RCC2 |=  0x80000000;  // USERCC2
  SYSCTL->RCC2 |=  0x00000800;  // BYPASS2, PLL bypass
  SYSCTL->RCC  = (SYSCTL->RCC &~ 0x000007C0) + 0x00000540;  // clear XTAL field, bits 10-6  // 10101, configure for 16 MHz crystal
  SYSCTL->RCC2 &= ~0x00000070;  // configure for main oscillator source
  SYSCTL->RCC2 &= ~0x00002000;// 3) activate PLL by clearing PWRDN
  SYSCTL->RCC2 |= 0x40000000;   // 4) set the desired system divider  use 400 MHz PLL
  SYSCTL->RCC2 = (SYSCTL->RCC2 &~ 0x1FC00000) + (24<<22);      // configure for 16 MHz clock / / clear system clock divider +
  while((SYSCTL->RIS&0x00000040)==0){} // 5) wait for the PLL to lock by polling PLLLRIS  // wait for PLLRIS bit
  SYSCTL->RCC2 &= ~0x00000800;// 6) enable use of PLL by clearing BYPASS
}

void PC4_as_M1PWM3_Init(void){
	// Step 1: Clock enable on PortC
	SYSCTL->RCGCGPIO |= 0x04;		//  0000 0100 
	for (j =0; j < 20 ; j++)			// at least 3 clock cyles
	GPIOC->AFSEL |= 0x10;// Step 3: Enable alternate functionality on PortC	 0001 0000
	GPIOC->DEN |= 0x10; // Step 4: Enable digital pin functionaliy on PortC pin 4
	GPIOC->DIR |= 0x10;// Step 5: Set PortC pin 4 as an output pin
	// Step 6: Configure PortC pin 4 as M1PWM6 pin (Table 10-2 of Datasheet, page # 651)
	GPIOC->PCTL &= 0xFFF0FFFF;		// clear the bit fields
	GPIOC->PCTL |= 0x000F0000;
}

void PWM_Module1_Channel3_Init(void){
	
	// Step 1: Clock Gating Control of PWM Module 1
	SYSCTL->RCGCPWM |= 0x01;		// b 0000 0010
	for (j =0; j < 20 ; j++)			// at least 3 clock cyles
	SYSCTL->RCC	&= (~(1<<20));		// disable clock signal divisor for PWM
	
	// Step 2: For PWM Channel configurations
	// we need check which PWM block our PWM Pin blongs to. For our case PF2
	// is M1PWM6 which is a part of PWM block 3 
	// Read any register description for identifying the block, e.g., CTL
	
	// Step 3: Disable Generator 1 before performing configurations
	// Step 4: Select Counter 1 Count-down mode
	PWM0->_1_CTL = 0x00;
	
	// Step 5: Set Load value for 10 kHz
	// (16 MHz / 10 kHz = 1600)
	PWM0->_1_LOAD = 1600;
	
	// Step 6: Set Compare Register Value to set 50% duty cycle
	// 50% of Load value = 1600 x 50% = 1600 x 0.5 = 800
	PWM0->_1_CMPA = 800;
	
	// Step 7: Perform Generating Control Register configuration
	// PWM signal HIGH when counter relaoded and LOW when matches CMPA Value
	PWM0->_1_GENA |= 0x8C;
	
	// Step 8: Enable generator 1 counter
	PWM0->_1_CTL |= 0x01;
	
	// Step 9: Enalbe PWM Module 0 Channel 3 Output
	PWM0->ENABLE |= 1<<3;
}
