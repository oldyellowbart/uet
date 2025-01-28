// uart interface
SYSCTL->RCGCUART |= 1; // enable clock for UART0
SYSCTL->RCGCUART |= 2;  // enable clock for UART1
GPIOA->DEN = 0x03; /* Make PA0 and PA1 as digital */
GPIOA->AFSEL = 0x03;/* Use PA0,PA1 alternate function */
GPIOA->PCTL |= 0x00000011 // PA0 and PA1 configure for UART module
GPIOD->PCTL |= 0x11000000 // PD6 and PD7 configure for UART2
9600 =  (16MHz / 16 x baud divisor) 
Baud divisor = 1000000/9600 = 104.1667
UART0->IBRD = 104;/* 16MHz/16=1MHz, 1MHz/104=9600 baud rate */

#include "TM4C123.h"
#include <stdint.h>
#include <stdlib.h>
void Delay(unsigned long counter);
char UART5_Receiver(void);
void UART5_Transmitter(unsigned char data);
void printstring(char *str);
int main(void)
{
	SYSCTL->RCGCUART |= 0x20;  /* enable clock to UART5 */
    SYSCTL->RCGCGPIO |= 0x10;  /* enable clock to PORTE for PE4/Rx and RE5/Tx */
    Delay(1);
    /* UART0 initialization */
    UART5->CTL = 0;         /* UART5 module disbable */
    UART5->IBRD = 104;      /* for 9600 baud rate, integer = 104 */
    UART5->FBRD = 11;       /* for 9600 baud rate, fractional = 11*/
    UART5->CC = 0;          /*select system clock*/
    UART5->LCRH = 0x60;     /* data lenght 8-bit, not parity bit, no FIFO */
    UART5->CTL = 0x301;     /* Enable UART5 module, Rx and Tx */

    /* UART5 TX5 and RX5 use PE4 and PE5. Configure them digital and enable alternate function */
    GPIOE->DEN = 0x30;      /* set PE4 and PE5 as digital */
    GPIOE->AFSEL = 0x30;    /* Use PE4,PE5 alternate function */
    GPIOE->AMSEL = 0;    /* Turn off analg function*/
    GPIOE->PCTL = 0x00110000;     /* configure PE4 and PE5 for UART */
	
	  Delay(1); 
	printstring("Hello World \n");
	Delay(10); 
	while(1)
	{
		char c = UART5_Receiver();          /* get a character from UART5 */
		UART5_Transmitter(c); 
	}
}

char UART5_Receiver(void)  
{
    char data;
	  while((UART5->FR & (1<<4)) != 0); /* wait until Rx buffer is not full */
    data = UART5->DR ;  	/* before giving it another byte */
    return (unsigned char) data; 
}

void UART5_Transmitter(unsigned char data)  
{
    while((UART5->FR & (1<<5)) != 0); /* wait until Tx buffer not full */
    UART5->DR = data;                  /* before giving it another byte */
}

void printstring(char *str)
{
  while(*str)
	{
		UART5_Transmitter(*(str++));
	}
}

void Delay(unsigned long counter)
{
	unsigned long i = 0;
	
	for(i=0; i< counter; i++);
}
void SystemInit(void)
{
    __disable_irq();    /* disable all IRQs */
    
    /* Grant coprocessor access */
    /* This is required since TM4C123G has a floating point coprocessor */
    SCB->CPACR |= 0x00F00000;
}















#include "TM4C123.h"
void Delay(unsigned long counter);
char UART5_Receiver(void);
void UART5_Transmitter(char data);
int main(void)
{
	
	while(1)
	{
		
         
                UART5_Transmitter('E');
                UART5_Transmitter('L'); 
                UART5_Transmitter('L'); 
                UART5_Transmitter('O'); 
                UART5_Transmitter('\n');
	}
}


void UART5_Transmitter(char data)  
{
    while((UART3->FR & 0x20) != 0); /* wait until Tx buffer not full */
    UART3->DR = data;                  /* before giving it another byte */
}

void Delay(unsigned long counter)
{
	unsigned long i = 0;
	
	for(i=0; i< counter; i++);
}
void Uart3_Init()
{
    //SYSCTL->RCGCGPIO |= 0x04;  // enable clock to PORTE for PE4/Rx and RE5/Tx 00 0100
    SYSCTL->RCGCUART |= 0x01;  // enable clock to UART3 0000 1000
    Delay(1);
    //--------------------------------------------------------------------------------//
    // UART3 initialization 
    UART3->CTL = 0;         // UART3 module disbable 
    UART3->IBRD = 104;      // for 9600 baud rate, integer = 104 
    UART3->FBRD = 11;       // for 9600 baud rate, fractional = 11
    UART3->CC = 0;          //select system clock
    UART3->LCRH = 0x60;     // data lenght 8-bit, not parity bit, no FIFO 
    UART3->CTL = 0x301;     // Enable UART5 module, Rx and Tx 

    // UART5 TX3 and RX3 use PC7 and PC6. Configure them digital and enable alternate function 
    GPIOC->DEN = 0xC0;      // set PC6 and PC7 as digital (1100 0000) 
    GPIOC->AFSEL = 0xC0;    // Use PC6,PC7 alternate function (1100 0000)
    GPIOC->AMSEL = 0;    // Turn off analg function
    GPIOC->PCTL = 0x00110000;     // configure PC6 and PC7 for UART 
	
	  Delay(1); 
	
}