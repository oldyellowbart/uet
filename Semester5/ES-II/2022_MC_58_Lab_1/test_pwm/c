/* Lab_07_task_2 task recreate using library & Example
-----------------------------------------------------------------------
Task 7.2: Interface a BLDC Motor with TIVA board by using any PWM 
channel and program it to run at average speed, continuously. 
-----------------------------------------------------------------------
Written by Haseeb-ul-hassan (github-oldyellowbart) on 15-SEP-2024.
Compiler and IDE used: Keil uVision 5.31a */
//_______________________________________________________________________
// Libraries
#include "TM4C123.h"                    // Device header

unsigned volatile long j;
volatile uint32_t i;

unsigned volatile long jk;
void PLL_Init(void);
void PF2_as_M1PWM6_Init(void);
void PWM_Module1_Channel6_Init(void);
void clocks_delay(volatile uint32_t clocks);

int main(){
	PLL_Init();
	PF2_as_M1PWM6_Init();
	PWM_Module1_Channel6_Init();
	
	while(1){
		PWM1->_3_CMPA = 36335;//for averahe bldc pwm
	}
}

void clocks_delay(volatile uint32_t clocks){
	while(clocks --);
}

// Configure the system clock to 16 MHz using PLL-------------------------------
void PLL_Init(void){

  // 0) Use RCC2
  SYSCTL->RCC2 |=  0x80000000;  // USERCC2

  // 1) bypass PLL while initializing
  SYSCTL->RCC2 |=  0x00000800;  // BYPASS2, PLL bypass

  // 2) select the crystal value and oscillator source
  SYSCTL->RCC = (SYSCTL->RCC &~ 0x000007C0) + 0x00000540;  // clear XTAL field, bits 10-6  // 10101, configure for 16 MHz crystal
  SYSCTL->RCC2 &= ~0x00000070;  // configure for main oscillator source

  // 3) activate PLL by clearing PWRDN
  SYSCTL->RCC2 &= ~0x00002000;

  // 4) set the desired system divider
  SYSCTL->RCC2 |= 0x40000000;   // use 400 MHz PLL
  SYSCTL->RCC2 = (SYSCTL->RCC2 &~ 0x1FC00000) + (24<<22);      // configure for 16 MHz clock / / clear system clock divider +

  // 5) wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL->RIS&0x00000040)==0){}  // wait for PLLRIS bit

  // 6) enable use of PLL by clearing BYPASS
  SYSCTL->RCC2 &= ~0x00000800;
}

void PF2_as_M1PWM6_Init(void){
	// Step 1: Clock enable on PortF
	SYSCTL->RCGCGPIO |= 0x20;		// 0b 0010 0000 
	for (j =0; j < 3 ; j++)			// at least 3 clock cyles
	
	// Step 2: APB is selected for PortF by selecting
	// 0x40025000 as Base Address in DATA section
	
	// Step 3: Enable alternate functionality on PortF
	GPIOF->AFSEL |= 1<<2;				// 0b 0000 0100
	
	// Step 4: Enable digital pin functionaliy on PortF pin 2
	GPIOF->DEN |= 1<<2; // Digital enable for PF2
	
	// Step 5: Set PortF pin 2 as an output pin
	GPIOF->DIR |= 1<<2; // PF2 as Output pin
	
	// Step 6: Configure PortF pin 2 as M1PWM6 pin (Table 10-2 of Datasheet, page # 651)
	GPIOF->PCTL &= 0xFFFFF0FF;		// clear the bit fields
	GPIOF->PCTL |= 0x00000500;
}

void PWM_Module1_Channel6_Init(void){
	
	// Step 1: Clock Gating Control of PWM Module 1
	SYSCTL->RCGCPWM |= (1<<1);		// b 0000 0010
	for (j =0; j < 10 ; j++)			// at least 3 clock cyles
	SYSCTL->RCC	&= ((0x00140000));		// disable clock signal divisor for PWM
	
	// Step 2: For PWM Channel configurations
	// we need check which PWM block our PWM Pin blongs to. For our case PF2
	// is M1PWM6 which is a part of PWM block 3 
	// Read any register description for identifying the block, e.g., CTL
	
	// Step 3: Disable Generator 3 before performing configurations
	// Step 4: Select Counter 3 Count-down mode
	PWM1->_3_CTL = 0x00;
	
	// Step 5: Set Load value for 10 kHz
	// (1.28 MHz / 10 kHz = 1600)
	PWM1->_3_LOAD = 38450;
	
	// Step 6: Set Compare Register Value to set 50% duty cycle
	// 50% of Load value = 38540 x 10-5% =  3845-1927
	PWM1->_3_CMPA = 34605;// 34605-2ms 36572-1ms
	
	// Step 7: Perform Generating Control Register configuration
	// PWM signal HIGH when counter relaoded and LOW when matches CMPA Value
	PWM1->_3_GENA |= 0x8C;
	
	// Step 8: Enable generator 3 counter
	PWM1->_3_CTL |= 0x01;
	
	// Step 9: Enalbe PWM Module 1 Channel 6 Output
	PWM1->ENABLE |= 1<<6;
}
