#include "TM4C123.h"                    // Device header
char data;
unsigned volatile long j;
uint8_t volatile Button_State, Button_State1;

void clocks_delay(volatile uint32_t clocks){
	while(clocks--);
}
void PF3_Digital_Output_Init(void);
void PF0_4_Digital_Inputs_Init(void);
void UART0_Tx_RX_Init(void);
void PA0_1_as_UART_Tx_Rx_Init(void);

int main(){
	PF3_Digital_Output_Init();
	PF0_4_Digital_Inputs_Init();
	UART0_Tx_RX_Init();
	PA0_1_as_UART_Tx_Rx_Init();
	while(1){
		data = UART0->DR;
		while((UART0->FR&(1<<4)) != 0);
		if(data == 'a'){
					// Transmit FIFO is not full
			
			GPIOF->DATA |= (1<<3);	// LED ON
			clocks_delay(3000000);
		}
		while((UART0->FR&(1<<4)) != 0);
		if(data == 'b'){
					// Transmit FIFO is not full
			
			GPIOF->DATA &= ~(1<<3);	// LED OFF
			clocks_delay(3000000);
		}
	}
}

void PF3_Digital_Output_Init(){
	// Step 1: Clock enable on PortF
	SYSCTL->RCGCGPIO |= 0x20;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: APB is selected for PortF by selecting
	// 0x40025000 as Base Address in DATA section
	
	// Step 3: Disable alternate functionality on PortF
	GPIOF->AFSEL |= 0x00;
	
	// Step 4: Enable digital pin functionaliy on PortF pin 3
	GPIOF->DEN |= 0x08; // Digital enable for PF3
	
	// Step 5: Set PortF pin 3 as an output pin
	GPIOF->DIR |= 0x08; // PF3 as output
}

void PF0_4_Digital_Inputs_Init(){
	// Step 1: Clock enable on PortF
	SYSCTL->RCGCGPIO |= 0x20;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: APB is selected for PortF by selecting
	// 0x40025000 as Base Address in DATA section
	
	// Unlock the PortF Pin4
	GPIOF->LOCK = 0x4C4F434B;
	GPIOF->CR = 0x01;
	GPIOF->LOCK = 0;
	
	// Step 3: Disable alternate functionality on PortF
	GPIOF->AFSEL |= 0x00;
	
	// Step 4: Enable digital pin functionaliy on PortF pin 0&4
	GPIOF->DEN |= 0x11; // Digital enable for PF0&4
	
	// Step 5: Set PortF pin 0&4 as an input pin
	GPIOF->DIR &= ~0x11; // PF0&4 as input
	GPIOF->PUR |= 0x11;		// Use internal weak pull-up resistors
}

void UART0_Tx_RX_Init(void){
	// UART0 -> PA0 (Rx), PA1	(Tx) (Virtual COM Port available on TIVA)
	// If we use any other UART module then we will need to use
	// TTL converter or UART to USB converter while connecting to PC
	
	// Step 1: Enable Clock Gating Control
	SYSCTL->RCGCUART |= 0x01;			// Enable clock on UART0
	for (j =0; j < 3 ; j++);		// at least 3 clock cyles
	
	// Step 2: Ensure UART is disabled before performing configurations
	UART0->CTL = 0x00;		// Disable UART, Tx, Rx
	
	// Step 3: Set UART Baud Rate
	UART0->IBRD = 104;		// BRD = UART_Sysclk/(ClkDiv*Buad Rate)
												// BRD = 16,000,000/(16*9600) = 104.166666667
	UART0->FBRD = 11;			// FBRD = BRDF *64+0.5 = 0.166666667*64+0.5
												// FBRD = 11.1666666667
	
	// Step 4: Perform Line Configurations
	UART0->LCRH |= 0x60;	// 8-bit word length
	
	// Step 5: Select Clock Source
	UART0->CC |= 0x05; 		// Use PIOSC as a source for UART Baud clock
	
	// Step 6: Turn On UART Module
	UART0->CTL |= 0x301;	// Turn on UART EN & TxEN & RxEN bits   
}

void PA0_1_as_UART_Tx_Rx_Init(void){
	// Step 1: Clock enable on PortF
	SYSCTL->RCGCGPIO |= 0x01;		// 0b 0000 0001
	for (j =0; j < 3 ; j++);			// at least 3 clock cyles
	
	// Step 2: APB is selected for PortA by selecting
	// 0x40025000 as Base Address in DATA section
	
	// Step 3: Enable alternate functionality on PortA
	GPIOA->AFSEL |= 0x03;				// 0b 0000 0011
	
	// Step 4: Enable digital pin functionaliy on PortA pin0-1 
	GPIOA->DEN |= 0x03; // Digital enable for PA0-1
	
	// Step 5: Set PortA pin 0-1 as input/output pins
	GPIOA->DIR &= ~0x01; // PA0 as Input pin (Rx)
	GPIOA->DIR |= 0x02;  // PA1 as Output pin (Tx)
	
	// Step 6: Configure PortA pin 0-1 as UART0 pin (Table 14-2 of Datasheet, page # )
	GPIOA->PCTL &= 0xFFFFFF00;		// clear the bit fields
	GPIOA->PCTL |= 0x00000011;
}