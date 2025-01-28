/* 
-----------------------------------------------------------------------
** Interfacing details---------
RS   -> PA2
R/W  -> PA3
EN   -> PA4
D0-7 -> PB0-PB7
PE2  -> ADC0_in 0-3.3v 0-4096
PE1  -> pwm generation //pe4
PE0  -> ADC1,SS0,250Ks,Current sensor
Uart5 -> PE4,PE5
optocoupler-> pc4
-----------------------------------------------------------------------*/
// Libraries
#include "TM4C123.h"
#include <string.h>
#include <stdio.h>
//------------------------------------------------------------------------//
// Global Variables
int n[10] = {48,49,50,51,52,53,53,54,55,56};//Array for numbers
int l=0; 
float current;
uint32_t edge_count = 0;
unsigned volatile long i;
unsigned volatile long j;
unsigned volatile long k;
unsigned int adc_data = 48;
unsigned int adc_data2 = 48;
unsigned volatile long pwm =0;
unsigned volatile long pwm2 =0;
//---------------------------------------------------------------------------//
// Function Declarations
void PF2_as_M1PWM6_Init(void);
void PWM_Module1_Channel6_Init(void);
void PortA23_as_Output_Init(void);
void PortB_as_Output_Init(void);
void PLL_Init(void);
void PortE_as_analog_Input_Init(void);
void write_LCD_Char(int data);
void write_LCD_number(int number);
void send_LCD_Cmd(unsigned char cmd);
void write_LCD_Str(unsigned char *str);
char UART5_Receiver(void);
void UART5_Transmitter(char data);
void LCD_Init(void);
void Uart5_Init(void);
void pwm_generate(int pwm_value);
void set_cursor(int column,int row);
void display(int adc1 , int pwm1,int edge,int current);
void msdelay(int time1);
void usdelay(int time);
void PC4_as_WT2CCP0_Init(void);
void WTIMER0A_RisingEdgeEvent_Init(void);
int WTIMER0A_RisingEdgeEvent_Capture(void);
int calculateLength(int number);
//------------------------->(Main Function)<-------------------------// 
int main()
	{
	//PLL_Init();
	PortB_as_Output_Init();
	PortA23_as_Output_Init();
	PortE_as_analog_Input_Init();
	PC4_as_WT2CCP0_Init();// Initialize PortD pin 2 as T3CCP0
	WTIMER0A_RisingEdgeEvent_Init();// Initialize WTIMER2A as Rising Edge Event Counter
	Uart5_Init();
	LCD_Init();
	//PF2_as_M1PWM6_Init();
	//PWM_Module1_Channel6_Init();
	while(1)
		{	
		//int test = 0;
		edge_count = WTIMER0A_RisingEdgeEvent_Capture();
	  adc_data = ( ADC0->SSFIFO3 & 0xFFF);//ADC_SS3_FIFO_DATA_R & 0xFFF 0.5us
		pwm = (((adc_data * 0.263)+1000));//122.07 for 1sec0.5us
		adc_data2 = ( ADC1->SSFIFO0 & 0xFFF);//ADC_SS0_FIFO_DATA_R & 0xFFF0.5us
		//current = (952-(4096-adc_data2))*33.33;
			current = ((4096-adc_data2));
		//test = ((adc_data*1.7)+2000);
		//PWM1->_3_CMPA = test;
		//PWM1->_3_CMPA = (adc_data*16);
		pwm_generate(pwm);
		display(adc_data,pwm,edge_count,current);	
	}
	}
	//------------------------->(Function Defintions)<-------------------------//
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
void PortB_as_Output_Init(void)
{
	SYSCTL->RCGCGPIO |= 0x02;// Step 1: Clock enable on PortB 00 0010
	usdelay(1);						   // Step 2: wait at least 3 clock cyle
	GPIOB->AFSEL &= (0x00);  // Step 3: Disable alternate functionality on PortB (0000 0000)
	GPIOB->DEN |= 0xFF;      // Step 4: Enable digital pin functionaliy on PortB pin 0-7(1111 1111)
	GPIOB->DIR |= 0xFF;			 // Step 5: Set PortB pin 0-7 as an output pin(1111 1111)
}
void PortA23_as_Output_Init(void)
{
	SYSCTL->RCGCGPIO |= 0x01;// Step 1: Clock enable on PortB 00 0010
	usdelay(1);	   // Step 2: wait at least 3 clock cyle
	GPIOA->AFSEL &= (0x1C);  // Step 3: Disable alternate functionality on PortB (0000 1100)
	GPIOA->DEN |= 0x1C; 		 // Step 4: Enable digital pin functionaliy on PortB pin 0-7(1111 1111)
	GPIOA->DIR |= 0x1C;			 // Step 5: Set PortA pin 0-1 as an output pin(0000 1100)
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
	
	//-------------------------> ADC_0 <--------------------------------------------//
	
	ADC0->ACTSS 	&= (0x00);	// Step10 : sample sequensor are disabled
	ADC0->EMUX 		|= 0xF000;	// Step11 :	slect the trigger event for ss3(1111 0000 0000 0000)
	ADC0->SSMUX3 	|= 0x01;		// Step12 :	sample sequencor input selectiom
	ADC0->PP 			|= 0x03;		// Step13	: 250ksps rate select
	ADC0->SSPRI 	|= 0x2000;	// Step14	: 2 sequence priority(0010 0000 0000 0000)
	ADC0->SAC 		|= 0x08 ;		// Step15	: 2^4 == 16 means sample average of 16x(go upto 6 for 64x)( 0000 1000)
	ADC0->CTL 		|= 0x01;    // Step16	: ADC_SS3_CONTROL_R  Sample is end of sequence
	ADC0->SSCTL3 	|= 0X02;		// Step17	:
	ADC0->ACTSS 	|= (0x08);	// Step18	: turn on SS3(Sample Sequencer 3)
	
	//-------------------------> ADC_1 <--------------------------------------------//
	
	ADC1->ACTSS 	&= (0x00);	// Step19	: sample sequensor are disabled(ss3 ss2 ss1 ss0)=(0000 0000 0000 0000)=0x0000
	ADC1->EMUX 		|= 0x000F;	// Step20	:set the trigering event here i select continous triger  mode for ss0
	ADC1->SSMUX0 	|= 0x03;		// Step21	:sample sequencor input selectiom
	ADC1->PP 			|= 0x03;		// Step22 : 250ksps rate select
	ADC1->SSPRI 	|= 0x0003;	// Step23 :  sequence priority low
	ADC1->SAC 		|= 0x08 ;		// Step24 : 2^4 == 16 means sample average of 16x(go upto 6 for 64x)
	ADC1->CTL 		|= 0x01;		// Step25	: ADC_SS3_CONTROL_R  Sample is end of sequence
	ADC1->SSCTL0 	|= 0X02;		// Step26	:
	ADC1->ACTSS 	|= 0x01;		// Step27	:	turn on ss0
}
void write_LCD_Char(int data)
{
	// Interfacing details---------------
	// RS -> A2		R/W -> PA3			EN -> PA4			D0-D7 -> PB0-PB7
	GPIOB->DATA &= (0x00);  	// Step 1. Erase previous data
	GPIOB->DATA |= data;			// Step 2. Pass data to 8-bit lines of LCD (D0-7)
	GPIOA->DATA &= ~(0x08); 	// Step 3. Enable write operation on LCD (R/W bit = 0) PA3 = 0000 1000
	GPIOA->DATA |= 0x04;			// Step 4. Select Data Register of LCD (RS bit = 1)	PA2 = 0000 1000
	GPIOA->DATA |= 0x10;			// Step 5. Enable LCD operation by sending high to low pulse on Enable pin	PA4 = 0001 0000
	for (j =0;j<500;j++);			// Step 6. some milisecond delay to match lcd speed of writing data
	GPIOA->DATA &= ~(0x10);		// Step 7. Disabled the enable for next time PA4 = 0001 0000
}

void send_LCD_Cmd(unsigned char cmd){
	// Interfacing details---------------
	// RS -> A2		R/W -> PA3			EN -> PA4			D0-D7 -> PB0-PB7
	GPIOB->DATA = cmd;		 		// Step 1. Pass command to 8-bit lines of LCD (D0-7)
	GPIOA->DATA &= ~(0x08);		// Step 2. Enable write operation on LCD (R/W bit = 0) PA3 = 0000 1000
	GPIOA->DATA &= ~(0x04);		// Step 3. Select Instructioin Register of LCD (RS bit = 0)	PA2 = 0000 0100
	GPIOA->DATA |= 0x10;	 		// Step 4. Enable LCD operation by sending high to low pulse on Enable pin 	PA4 = 0001 0000
	for (j =0; j <500 ; j++); // Step 5. some milisecond(10us) delay to match lcd speed of writing data
	GPIOA->DATA &= ~(0x10); 	// Step 6. Disabled for next time PA4 = 0001 0000
}

//Function to print String

void write_LCD_Str(unsigned char *str)
	{
	unsigned int i, len;
	len = strlen(str);
	for (i = 0; i < len; i++){write_LCD_Char(str[i]);}
	}

void LCD_Init(void)
	{
	send_LCD_Cmd(0x38);//Step1: Function Set: 8-bit, 2 Line, 5x7 Dots
	send_LCD_Cmd(0x06);//Step2: Auto increment cursor (shift cursor to right)
	send_LCD_Cmd(0x0C);//Step3: Display on, cursor off
	send_LCD_Cmd(0x01);//Step4: Clear display screen
	send_LCD_Cmd(0x80);//Step5: Force cursor to the beginning (1st line)	
}

// Function to calculate length of the given number

int calculateLength(int number)
	{
    int length = 0;
    if (number == 0) return 1; // Special case for 0
    while (number != 0){number /= 10;length++;}
    return length;
	}
	
// Function to convert an integer to its individual digits and display on LCD
	
void write_LCD_number(int number)
	{
    int length = calculateLength(number);
    char digits[length + 1]; 						 // Array to hold the individual digits as characters
    for (int i = length - 1; i >= 0; i--)// Extract each digit from the end
		{
      digits[i] = (number % 10) + '0';	 // Convert digit to character
      number /= 10;										   // Get the number 
    }
    digits[length] = '\0'; // Null terminator
    for (int i = 0; i < length; i++){write_LCD_Char(digits[i]);}// Write each digit to the LCD
	
}
void usdelay(int time){for (i = 0; i < time*4; i++);}
void msdelay(int time1){for (i = 0; i < time1*4000; i++);}

void set_cursor(int row,int column)//sets the cursor line & position
{
	if(row == 1){send_LCD_Cmd(0x80);}// switch to line 1 
	if(row == 2){send_LCD_Cmd(0xC0);}// switch to line 2
	if(row == 3){send_LCD_Cmd(0x94);}// switch to line 3
	if(row == 4){send_LCD_Cmd(0xD4);}// switch to line 4
for(k=0 ; k < column ; k++)				 //	Set cursor position
{
	send_LCD_Cmd(0x14);}						 // Shift cursor to write 
}

void display(int adc1 , int pwm1,int edge,int current)
{			
			send_LCD_Cmd(0x01);
			usdelay(1400);// delay for 1 charachter to write is 400ms
			set_cursor(1,0);
			write_LCD_Str("adc1 =");
			set_cursor(1,12);
			write_LCD_number(adc1);
			set_cursor(2,0);
			write_LCD_Str("pwm_onn =");
			set_cursor(2,12);
		  write_LCD_number(pwm1);
			set_cursor(3,0);
			write_LCD_Str("Edge_Count =");
			set_cursor(3,12);
			write_LCD_number(edge);
			set_cursor(4,0);
			write_LCD_Str("Current =");
			set_cursor(4,12);
			write_LCD_number(current);	
}

void pwm_generate(int pwm_value)
{
		GPIOE->DATA |= 0x02;// Turn PE1 High
		usdelay(pwm_value); // Delay required to set servo at 0 degrees
		GPIOE->DATA &= ~(0x02);// Turn PE1 Low
		usdelay(12000-pwm_value);//
		
}
void PC4_as_WT2CCP0_Init(void){
	SYSCTL->RCGCGPIO |= 0x04; 				// Step 1: Clock enable on PortC 00 0100
	usdelay(1);												// Step 2: Wait at least 3 clock cyles
	GPIOC->AFSEL		 |= 0x10;   			// Step 3: Enable alternate functionality on PortB(0001 0000)
	GPIOC->DEN 			 |= 0x10;    			// Step 4: Enable digital pin functionaliy on PortB pin 2 
	GPIOC->DIR 			 &= ~(0x10);			// Step 5: Set PortB pin 2 as an output pin
	GPIOC->PCTL 		 &= ~0x00070000;	// Step 6: Clear the bit fields
	GPIOC->PCTL 		 |= 0x00070000;		// Step 7: Configure PortB pin 2 as T3CCP0 pin (Table 10-2 of Datasheet, page # 651)
}
void WTIMER0A_RisingEdgeEvent_Init(void){
	SYSCTL->RCGCWTIMER |=0x01;		// Step 1: Enable Timer Clock on WTIMER0 (0000 0001)
	for (j =0; j < 3 ; j++)				// wait at least 3 clock cyles
	WTIMER0->CTL = 0x00;					// Step 2: Ensure Timer is disabled before making any changes TAEN = 0, i.e., WTIMER0A is disablled
	// Step 3: Select Mode of Operation of WTIMER0 (Split/cancatenated/RTC)
	WTIMER0->CFG = 0x04;					// WTIMER0 is used as a 16-bit (split) timer, i.e., WTIMER0A
	WTIMER0->TAMR = 0x13;					// TAMR = 3 (capture), TACMR = 0 (edge-count) TACDIR = 1 (count-up)
	// Step 4: Set counter value limit, comparted to TAR to determine match event
	WTIMER0->TAMATCHR = 0xFFFF;		// 
	WTIMER0->TAPMR = 0xFF;				// used with TAMATCHR to expand to 0xFFFFFF with 8-bit prescaler (bit 16:23)
	// Step 5: Interrupt configurations
	WTIMER0->ICR = 1<<0;					// Clear timer status flag (TATORIS, TATOMIS)
	// Step 6: Enable the Timer and start counting
	WTIMER0->CTL |= 1<<0;					// TAEN = 1
}
int WTIMER0A_RisingEdgeEvent_Capture(void)
{
	return WTIMER0->TAR;	// Compared with TAMATCHR to determine match event it contains the number of edges that have occurred
}

// nrpm r/60s;n/60000->rounds/60ms;r/1000-> round per 1 ms
//timer off flag clear timer on
void Uart5_Init()
{
		SYSCTL->RCGCUART |= 0x20; 		// enable clock to UART5 (0010 0000)
		SYSCTL->RCGCGPIO |= 0x10; 		// enable clock to PORTE for PE4/Rx and RE5/Tx 
    usdelay(1);										// 4 clock cycle delay to enable clock
	
 //---------------------------------> UART5 initialization <------------------------------------------------------//
  
		UART5->CTL 		&= 0;         	// UART5 module disbable 							(0000 0000)
    UART5->IBRD 	|= 104;      		// for 9600 baud rate, integer = 104 	
    UART5->FBRD 	|= 11;       		// for 9600 baud rate, fractional = 11
    UART5->CC 		|= 0;          	//select system clock
    UART5->LCRH 	|= 0x60;     		// data lenght 8-bit, not parity bit, no FIFO 
    UART5->CTL 		|= 0x301;     	// Enable UART5 module, Rx and Tx 
    GPIOE->DEN 		|= 0x30;      	// set PE4 and PE5 as digital 
    GPIOE->AFSEL	|= 0x30;    		// Use PE4,PE5 alternate function 
    GPIOE->AMSEL	&= 0;    	 			// Turn off analg function
    GPIOE->PCTL 	|= 0x00110000;	// configure PE4 and PE5 for UART 
}
void UART5_Transmitter(char data)  
{
  while((UART5->FR & 0x20) != 0); // wait until Tx buffer not full 
  UART5->DR |= data;              // before giving it another byte 
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
	SYSCTL->RCC	&= (~(1<<20));		// disable clock signal divisor for PWM
	
	// Step 2: For PWM Channel configurations
	// we need check which PWM block our PWM Pin blongs to. For our case PF2
	// is M1PWM6 which is a part of PWM block 3 
	// Read any register description for identifying the block, e.g., CTL
	
	// Step 3: Disable Generator 3 before performing configurations
	// Step 4: Select Counter 3 Count-down mode
	PWM1->_3_CTL = 0x00;
	
	// Step 5: Set Load value for 10 kHz
	// (16 MHz / 50 Hz = 3,20,000)
	PWM1->_3_LOAD = 320000;//6500-
	
	// Step 6: Set Compare Register Value to set 50% duty cycle
	// 50% of Load value = 1600 x 50% = 1600 x 0.5 = 800
	PWM1->_3_CMPA = 100;
	
	// Step 7: Perform Generating Control Register configuration
	// PWM signal HIGH when counter relaoded and LOW when matches CMPA Value
	PWM1->_3_GENA |= 0x8C;
	
	// Step 8: Enable generator 3 counter
	PWM1->_3_CTL |= 0x01;
	
	// Step 9: Enalbe PWM Module 1 Channel 6 Output
	PWM1->ENABLE |= 1<<6;
}


/*Task 2.1: Modify and Lab_02_Example_01 program to turn ON on-board
green LED on falling edge interrupt on PF4 and turn OFF it when rising
edge interrupt is received at PF0. You need to unlock PF0 before you 
can configure it.*/
#include "TM4C123.h"          
unsigned volatile long j;
void PF3_Digital_Output_Init();
void PF4_External_Interrupt_Input_Init();
void PF0_External_Interrupt_Input_Init();

// Main Function
int main()
	{
    PF3_Digital_Output_Init();             // Initialize PF3 (LED)
    PF4_External_Interrupt_Input_Init();  // Initialize PF4 (Switch 1)
    PF0_External_Interrupt_Input_Init(); // Initialize PF0 (Switch 2)
    while(1){}
	}

void GPIOF_Handler()
	{
    if((GPIOF->MIS & (0x10)) == (0x10))// Handle PF4 (Switch 1) interrupt - Turn ON LED (PF3)
			{  // PF4 interrupt occurred
        GPIOF->DATA |= (0x08);// Turn ON PF3 (LED)
        GPIOF->ICR |= 0x10;  // Clear interrupt flag for PF4
			}
    // Handle PF0 (Switch 2) interrupt - Turn OFF LED (PF3)
    if((GPIOF->MIS & (0x01)) == (0x01))
			{  // PF0 interrupt occurred
        GPIOF->DATA &= ~(0x08);// Turn OFF PF3 (LED)
        GPIOF->ICR |= 0x01;   // Clear interrupt flag for PF0
			}
	}
void PF3_Digital_Output_Init()
{
    SYSCTL->RCGCGPIO |= 0x20; // Step 1: Clock enable on PortF
    for (j = 0; j < 3; j++)  // at least 3 clock cycles delay
    GPIOF->AFSEL &= ~(0x08);// Step 2: Disable alternate function on PF3
    GPIOF->DEN |= 0x08;    // Step 3: Enable digital function on PF3
    GPIOF->DIR |= 0x08;   // Step 4: Set PF3 as an output pin
}

void PF4_External_Interrupt_Input_Init()
{
    SYSCTL->RCGCGPIO |= 0x20; // Step 1: Clock enable on PortF
    for (j = 0; j < 3; j++)  // at least 3 clock cycles delay
    GPIOF->AFSEL &= ~(0x10);// Step 2: Disable alternate function on PF4
    GPIOF->DEN |= 0x10;    // Step 3: Enable digital function on PF4
    GPIOF->DIR &= ~(0x10);// Step 4: Set PF4 as input pin
    GPIOF->PUR |= 0x10;  // Enable pull-up resistor on PF4
    // Step 5: Configure PF4 for falling edge interrupt
    GPIOF->IS &= ~(0x10);  // Edge-sensitive  
    GPIOF->IEV &= ~(0x10);// Falling edge
    GPIOF->IM |= 0x10;   // mask interrupt for PF4 intrept management port
    GPIOF->ICR |= 0x10; // Clear interrupt flag
    // Step 6: NVIC configuration for Port F interrupt (IRQ 30)
    NVIC->ISER[0] |= (1<<30);  // Enable IRQ for PortF esception number
    NVIC->IPR[7] = 0x00600000;// Set priority 3 for PortF interrupt 0000 0000 22,23,24
}
void PF0_External_Interrupt_Input_Init() 
	{
    SYSCTL->RCGCGPIO |= 0x20;   // Step 1: Clock enable on PortF
    for (j = 0; j < 3; j++)    // at least 3 clock cycles delay
    GPIOF->LOCK = 0x4C4F434B; // Step 2: Unlock PF0 (it is locked by default)
    GPIOF->CR |= 0x01;       // Enable commit for PF0
    GPIOF->AFSEL &= ~(0x01);// Step 3: Disable alternate function on PF0
    GPIOF->DEN |= 0x01;	   // Step 4: Enable digital function on PF0
    GPIOF->DIR &= ~(0x01);// Step 5: Set PF0 as input pin
    GPIOF->PUR |= 0x01;  // Enable pull-up resistor on PF0
    // Step 6: Configure PF0 for falling edge interrupt
    GPIOF->IS &= ~(0x01); // Edge-triggered
    GPIOF->IEV |= 0x01;  // Rising edge
    GPIOF->IM |= 0x01;  // Unmask interrupt for PF0
    GPIOF->ICR |= 0x01;// Clear interrupt flag
	}