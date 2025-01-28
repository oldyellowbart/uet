/* Lab_07_task_1 task recreate using library & Example
-----------------------------------------------------------------------
Task 7.1: Modify the Example_01 program such that you use some other
 PWM channel and external LED and control its brightness. .
-----------------------------------------------------------------------
Written by Haseeb-ul-hassan (github-oldyellowbart) on 15-SEP-2024.
Compiler and IDE used: Keil uVision 5.31a */
//_______________________________________________________________________
// Libraries
#include "TM4C123.h"                   // Device header
#include <string.h>
#include <stdio.h>
unsigned volatile long i;
unsigned volatile long j;
unsigned volatile long k;

#define Key_pad_Row_1 0x01;
void PLL_Init(void);
void PortB_as_Output_Init(void);
void clocks_delay(volatile uint32_t clocks);
void PE0123_as_Output_Init(void);//ok
void PC4567_as_Input_Init(void);
char UART5_Receiver(void);
void UART5_Transmitter(char data);
void usdelay(int time);
void keypad(void);
void Uart5_Init(void);
int calculateLength(int number);
int time =0;
void write_UART_number(int number);
void write_uart_Str(unsigned char *str1);
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
void PortB_as_Output_Init(void)
{
	SYSCTL->RCGCGPIO 		|= 0x02;						// Step 1: Clock enable on PortB 																(00 0010)
	usdelay(1);						   								// Step 2: wait at least 3 clock cyle
	GPIOB->AFSEL 		 		&= (0x00); 					// Step 3: Disable alternate functionality on PortB 						(0000 0000)
	GPIOB->DEN 			 		|= 0xFF;   					// Step 4: Enable digital pin functionaliy on PortB pin 0-7			(1111 1111)
	GPIOB->DIR 			 		|= 0xFF;						// Step 5: Set PortB pin 0-7 as an output pin										(1111 1111)
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
void write_UART_number(int number)
	{
    int length = calculateLength(number);
    char digits[length + 1]; 						 // Array to hold the individual digits as characters
    for (int i = length - 1; i >= 0; i--)// Extract each digit from the end
		{
      digits[i] = (number % 10) + '0';	 // Convert digit to character
      number /= 10;										   // Get the number 
    }
    digits[length] = '\0'; // Null terminator
    for (int iklm = 0; iklm < length; iklm++)
		{
		UART5_Transmitter(digits[iklm]);
		}// Write each digit to the LCD
}
	void write_uart_Str(unsigned char *str1)
	{
		unsigned int e, len;
		len = strlen(str1);
		for (e = 0; e < len; e++)
		{
		UART5_Transmitter(str1[i]);
		}
		UART5_Transmitter('\n');
	}
	
	void UART5_Transmitter(char data)  
{
  while((UART5->FR & 0x20) != 0); // wait until Tx buffer not full 
  UART5->DR |= data;              // before giving it another byte 
}
void Uart5_Init()
{
	SYSCTL->RCGCUART 		|= 0x20;	 					// Step 1: enable clock to UART5 																(0010 0000)
	SYSCTL->RCGCGPIO 		|= 0x10; 						// Step 2: enable clock to PORTE for PE4/Rx and RE5/Tx					(0001 0000) 
  usdelay(1);															// Step 3: 4 clock cycle delay to enable clock  			
	UART5->CTL 					&= 0;         			// Step 4: UART5 module disbable 																(0000 0000)
  UART5->IBRD			 		|= 104;      				// Step 5: for 9600 baud rate, integer = 104 										(0110 1000)
  UART5->FBRD				 	|= 11; 	      			// Step 6: for 9600 baud rate, fractional = 11									(0000 1011)
  UART5->CC 					|= 0;   	    	   	// Step 7: select system clock																	(0000 0000)
  UART5->LCRH 				|= 0x60;  	 		 		// Step 8: data lenght 8-bit, not parity bit, no FIFO 					(0110 0000)
  UART5->CTL 					|= 0x301;   	  		// Step 9: Enable UART5 module, Rx and Tx												() 
  GPIOE->DEN 					|= 0x30;		  	   	// Step 10: set PE4 and PE5 as digital 													(0011 0000)
  GPIOE->AFSEL				|= 0x30;    				// Step 11: Use PE4,PE5 alternate function 											(0011 0000)
  GPIOE->AMSEL				&= 0;    			 			// Step 12: Turn off analg function															(0000 0000)	
  GPIOE->PCTL 				|= 0x00110000;			// Step 13: configure PE4 and PE5 for UART 
}
void usdelay(int time){for (i = 0; i < time*4; i++);}