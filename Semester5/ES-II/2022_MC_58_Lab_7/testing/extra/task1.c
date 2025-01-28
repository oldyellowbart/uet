/* Example_7_1_LED_Intensity_Variation.uvprojx
-----------------------------------------------------------------------
This C language program controls the brightness of on-board user blue 
LED of TIVA C Series LaunchPad Evaluation Kit by Texas Instrument by 
using PWM Module 1 Channel 6 available at PortF Pin 2.
Note: Microcontroller operating frequency is set to 16 MHz using PLL.
-----------------------------------------------------------------------
Written by Shujat Ali (engrshujatali@gmail.com) on 28-Nov-2023.
Compiler and IDE used: Keil uVision 5.34 */
//e5m3
#include "TM4C123.h"                    // Device header

unsigned volatile long j;
volatile uint32_t i;

void PLL_Init(void);
void PF2_as_M1PWM6_Init(void);
void PWM_Module1_Channel6_Init(void);
void clocks_delay(volatile uint32_t clocks);

int main(){
	PLL_Init();
	PF2_as_M1PWM6_Init();
	PWM_Module1_Channel6_Init();
	
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

void PF2_as_M1PWM6_Init(void){
	// Step 1: Clock enable on PortF
	SYSCTL->RCGCGPIO |= 0x02;		// 0b 0001 0000 
	for (j =0; j < 20 ; j++)			// at least 3 clock cyles
	
	// Step 2: APB is selected for PortF by selecting
	// 0x40025000 as Base Address in DATA section
	
	// Step 3: Enable alternate functionality on PortF
	GPIOB->AFSEL |= 0x20;				// 0b 0010 0000
	
	// Step 4: Enable digital pin functionaliy on PortF pin 2
	GPIOB->DEN |= 0x20; // Digital enable for PF2
	
	// Step 5: Set PortF pin 2 as an output pin
	GPIOB->DIR |= 0x20; // PF2 as Output pin
	
	// Step 6: Configure PortF pin 2 as M1PWM6 pin (Table 10-2 of Datasheet, page # 651)
	GPIOB->PCTL &= 0xFF0FFFFF;		// clear the bit fields
	GPIOB->PCTL |= 0x00400000;
}

void PWM_Module1_Channel6_Init(void){
	
	// Step 1: Clock Gating Control of PWM Module 1
	SYSCTL->RCGCPWM |= 0x01;		// b 0000 0010
	for (j =0; j < 20 ; j++)			// at least 3 clock cyles
	SYSCTL->RCC	&= (~(1<<20));		// disable clock signal divisor for PWM
	
	// Step 2: For PWM Channel configurations
	// we need check which PWM block our PWM Pin blongs to. For our case PF2
	// is M1PWM6 which is a part of PWM block 3 
	// Read any register description for identifying the block, e.g., CTL
	
	// Step 3: Disable Generator 3 before performing configurations
	// Step 4: Select Counter 3 Count-down mode
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
	
	// Step 8: Enable generator 3 counter
	PWM0->_1_CTL |= 0x01;
	
	// Step 9: Enalbe PWM Module 1 Channel 6 Output
	PWM0->ENABLE |= 1<<3;
}
