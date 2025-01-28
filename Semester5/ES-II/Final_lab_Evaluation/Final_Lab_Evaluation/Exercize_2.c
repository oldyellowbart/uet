/* Lab_07_task_1 task recreate using library & Example
-----------------------------------------------------------------------
Task 7.1: Modify the Example_01 program such that you use some other
 PWM channel and external LED and control its brightness. .
-----------------------------------------------------------------------
Written by Haseeb-ul-hassan (github-oldyellowbart) on 15-SEP-2024.
Compiler and IDE used: Keil uVision 5.31a */
//_______________________________________________________________________
// Libraries
#include<TM4C123.h>                   // Device header

unsigned volatile long j;
volatile uint32_t i;
#define Key_pad_Row_1 0x01;
void PLL_Init(void);
void clocks_delay(volatile uint32_t clocks);
void PE0123_as_Output_Init(void);//ok
void PC4567_as_Input_Init(void);
void keypad(void);
unsigned char CC_7Seg_Disp_LookupTable[4][4] = { // col 1	 col2	 col3	 col4
	{0x79, 0x24, 0x30, 0x08},					 // row 1
	{0x19, 0x12, 0x02, 0x03},					 // row 2
	{0x78, 0x00, 0x10, 0x46},					 // row 3
	{0x92, 0x40, 0x09, 0x21}};					 // row 4
int press =0;
	int step = 0;
int main(){
	PLL_Init();

	
	while(1){
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
void PE0123_as_Output_Init(void)
					{
					// Step 1: Clock enable on PortE
					SYSCTL->RCGCGPIO |= 0x10;	// 01 0000
					for (j =0; j < 3 ; j++);		// wait at least 3 clock cyles
					// Step 2: APB is selected for PortE by selecting
					// 0x40007000 as Base Address in DATA section
					// Step 3: Disable alternate functionality on PortE
					GPIOE->AFSEL &= ~(0x0F); // Disable for PE0-3 (000 1111)
					// Step 4: Enable digital pin functionaliy on PortE pin 0-4
					GPIOE->DEN |= 0x0F; // Digital enable for PE0-4 (0001 1111)
					// Step 5: Set PortD pin 0-4 as an output pin
					GPIOE->DIR &= ~(0x0F); // PE0-4 as output (0000 0111)
					}
	void PC4567_as_Input_Init(void)
					{
					// Step 1: Clock enable on PortC
					SYSCTL->RCGCGPIO |= 0x04;	// 00 0100
					for (j =0; j < 3 ; j++)		// wait at least 3 clock cyles
					// Step 2: APB is selected for PortA by selecting
					// Step 3: Disable alternate functionality on PortB
					GPIOB->AFSEL &= ~(0xF0); // Disable for PF4567 (1111 0000)
					// Step 4: Enable digital pin functionaliy on PA2345 (0011 1100)
					GPIOB->DEN |= 0xF0; // Digital enable for PA2345 (0011 1100)
					// Step 5: Set PortB pin  as an output pin
					GPIOB->DIR &= ~(0xF0); // PA as Input PA2345 (0011 1100)
					}				
	void keypad(void)
{

					for (short j = 0; j < 4; j++)
					{GPIOE->DATA = 0x01<<j;
						for (short i = 0; i < 4; i++)
						{
						// Digital read for any key pressed on Keypad between 0 to 9
							if ((GPIOC->DATA & 0xF0) == 0x10 << i)
							{
								while ((GPIOC->DATA & 0xF0) == 0x01 << i){}
								step = CC_7Seg_Disp_LookupTable[j][i];}}
						}
}