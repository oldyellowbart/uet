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
volatile int rcc;
int l;
int rcc2;
int *dcb;
unsigned volatile long jk;
void PLL_Init(void);
void PF2_as_M1PWM6_Init(void);
void PWM_Module1_Channel6_Init(void);
void clocks_delay(volatile uint32_t clocks);
void pwm_generate(int pwm_value);
void PortE_as_analog_Input_Init(void);
void usdelay(int time);
void msdelay(int time1);

int main(){
	
	PF2_as_M1PWM6_Init();
	PWM_Module1_Channel6_Init();
	PortE_as_analog_Input_Init();
	PLL_Init();
	
	while(1){

		
	for(l=18000;l<19000;l++)
	{
	PWM1->_3_CMPA = l;
	usdelay(1000);
	}
		
	usdelay(500);
	for(l=19000; l>34605; l--)
	{
	PWM1->_3_CMPA = l;
	usdelay(1000);
	}
	usdelay(500);
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
	SYSCTL->RCC	= (0x00160000);		// Enable clock signal divisor for PWM 1Mhz
	// Step 2: For PWM Channel configurations
	// we need check which PWM block our PWM Pin blongs to. For our case PF2
	// is M1PWM6 which is a part of PWM block 3 
	// Read any register description for identifying the block, e.g., CTL
	rcc = ( SYSCTL->RCC);
		rcc2 = ( SYSCTL->RCC2);
	// Step 3: Disable Generator 3 before performing configurations
	// Step 4: Select Counter 3 Count-down mode
	PWM1->_3_CTL = 0x00;
	
	// Step 5: Set Load value for 10 kHz
	// (1.92Mhz / 50Hz = 38450)
	PWM1->_3_LOAD = 20000;
	
	// Step 6: Set Compare Register Value to set 50% duty cycle
	// 50% of Load value = 38540 x 10-5% =  3845-1927
	PWM1->_3_CMPA = 20000;// 34605-2ms 36572-1ms
	
	// Step 7: Perform Generating Control Register configuration
	// PWM signal HIGH when counter relaoded and LOW when matches CMPA Value
	PWM1->_3_GENA |= 0x8C;
	
	// Step 8: Enable generator 3 counter
	PWM1->_3_CTL |= 0x01;
	
	// Step 9: Enalbe PWM Module 1 Channel 6 Output
	PWM1->ENABLE |= 1<<6;
}
void pwm_generate(int pwm_value)
{
		GPIOE->DATA |= 0x02;// Turn PE1 High
		usdelay(pwm_value); // Delay required to set servo at 0 degrees
		GPIOE->DATA &= ~(0x02);// Turn PE1 Low
		usdelay(pwm_value);//
		
}
void PortE_as_analog_Input_Init(void)
{
	
	SYSCTL->RCGCGPIO |= 0x10;	// Step1  : Enable the clock for Port E 						(01 0000)
	usdelay(1);								// Step2  : Delay 4 clock cycle to enable clock 
	GPIOE->AFSEL 	|= (0x05);	// Step3  : Enable alternate on pe0,pe2 					(0000 0101)
	GPIOE->AMSEL 	&= ~(0x02); // Step4  : Disable_analog_mode_select_on PE1			(1111 1101)
	GPIOE->DEN 		|= 0x02;		// Step5  : Enable digital pin functionaliy				(0000 0010)
	GPIOE->DIR 		|= 0x02;		// Step6  : PE1 digital pins as an output 				(0000 0010)
	GPIOE->DIR		&= ~(0x05); // Step6  : PE0,PE1 as input 											(0000 0101)
	GPIOE->DEN 		&= ~(0x05); // Step6  : digital disable on pe2,pe0						(0000 0101)
	GPIOE->AMSEL 	|= 0x05;		// Step7  : analog enable on pe2,pe0							(0000 0101)
	SYSCTL->RCGCADC|= 0x03;	 	// Step8  : clock enable on adc 0,1 							(0000 0011)
	usdelay(3);								// Step9  : Delay to to enable clock

}

void usdelay(int time){for (i = 0; i < time*1.538; i++);}
void msdelay(int time1){for (i = 0; i < time1*1538; i++);}