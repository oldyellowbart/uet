/* 
-----------------------------------------------------------------------
** Interfacing details---------
RS   -> PA2
R/W  -> PA3
EN   -> PA4
D0-7 -> PB0-PB7
PE2  -> ADC0_in 0-3.3v 0-4096
PE1  -> pwm generation
PE0  -> ADC1,SS0,250Ks,Current sensor
uart -> c6,c7
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
unsigned volatile long j;
unsigned int adc_data = 48;
unsigned volatile long pwm =0;
unsigned int adc_data2 = 48;
unsigned volatile long pwm2 =0;
//---------------------------------------------------------------------------//
// Function Declarations
void PortA23_as_Output_Init(void);
void PortB_as_Output_Init(void);
void PortE_as_Output(void);
void PortE_as_analog_Input_Init(void);
void write_LCD_Char(int data);
void write_LCD_number(int number);
void send_LCD_Cmd(unsigned char cmd);
void write_LCD_Str(unsigned char *str);
void UART3_Transmitter(char data1);
void LCD_Init(void);
void Uart3_Init(void);
void pwm_generate(int pwm_value);
void set_cursor(int column,int row);
void display(int adc1 , int pwm1,int adc2);
void msdelay(int time1);
void usdelay(int time);
void Uart3_Init(void);
void PC4_as_WT2CCP0_Init(void);
void WTIMER0A_RisingEdgeEvent_Init(void);
int WTIMER0A_RisingEdgeEvent_Capture(void);
int calculateLength(int number);
//-----------------------------------------------------------------------------//
// Main Function
int main()
	{
	PortB_as_Output_Init();
	//PortE_as_Output();
	PortA23_as_Output_Init();
	PortE_as_analog_Input_Init();
	PC4_as_WT2CCP0_Init();// Initialize PortD pin 2 as T3CCP0
	WTIMER0A_RisingEdgeEvent_Init();// Initialize WTIMER2A as Rising Edge Event Counter
	Uart3_Init();
	LCD_Init();
		//write_LCD_Str("Pulse ");
		//set_cursor(1);
	while(1)
		{	
			//for( l = 0 ; l<500;l++){
		//edge_count = WTIMER0A_RisingEdgeEvent_Capture();
		UART3_Transmitter('H');
			UART3_Transmitter('H');
			UART3_Transmitter('H');
			UART3_Transmitter('H');
	 // adc_data = ( ADC0->SSFIFO3 & 0xFFF);//ADC_SS3_FIFO_DATA_R & 0xFFF 0.5us
		//pwm = (((adc_data * 0.263)+1000));//122.07 for 1sec0.5us
		//adc_data2 = ( ADC1->SSFIFO0 & 0xFFF);//ADC_SS0_FIFO_DATA_R & 0xFFF0.5us
		//current = (952-(4096-adc_data2))*33.33;
		//pwm_generate(pwm);
		
		//}
//display(adc_data,edge_count,current);	
	}
	}

// Function Defintions
void PortB_as_Output_Init(void){
	SYSCTL->RCGCGPIO |= 0x02;// Step 1: Clock enable on PortB 00 0010
	for (j =0;j< 3 ; j++)	   // Step 2: wait at least 3 clock cyle
	GPIOB->AFSEL &= (0x00);  // Step 3: Disable alternate functionality on PortB (0000 0000)
	GPIOB->DEN |= 0xFF;      // Step 4: Enable digital pin functionaliy on PortB pin 0-7(1111 1111)
	GPIOB->DIR |= 0xFF;			 // Step 5: Set PortB pin 0-7 as an output pin(1111 1111)
}
void PortA23_as_Output_Init(void){
	SYSCTL->RCGCGPIO |= 0x01;// Step 1: Clock enable on PortB 00 0010
	for (j =0;j< 3 ; j++)	   // Step 2: wait at least 3 clock cyle
	GPIOA->AFSEL &= (0x1C);  // Step 3: Disable alternate functionality on PortB (0000 1100)
	GPIOA->DEN |= 0x1C; 		 // Step 4: Enable digital pin functionaliy on PortB pin 0-7(1111 1111)
	GPIOA->DIR |= 0x1C;			 // Step 5: Set PortA pin 0-1 as an output pin(0000 1100)
}

void PortE_as_Output(void)
{
	//SYSCTL->RCGCGPIO |= 0x10;	// 01 0000
	for (j =0; j < 3 ; j++)	// wait at least 3 clock cyles
	GPIOE->AFSEL &= ~(0x02);//(0000 0010) Disable for PB0-7 (1111 1111)
	GPIOE->DEN |= 0x02; 	  // Digital enable for PB0-7 (1111 1111)
	GPIOE->DIR |= 0x02;     // Pe1 as output (0000 0001)
}
void PortE_as_analog_Input_Init(void)
{
	SYSCTL->RCGCGPIO |= 0x10;//Step1 : Enable the clock for Port  E
	usdelay(3);							 //Step2 : Delay to to enable clock 
	GPIOE->AFSEL |= (0x05);// alternate on pe0,pe2
	GPIOE->AMSEL &= ~(0x02); //Step3 : Disable_analog_mode_select_on PE1
	GPIOE->DEN |= 0x02;			 //Step4 : Enable digital pin functionaliy
	GPIOE->DIR |= 0x02;			 //Step5 : Enable digital pins as an output pe1
	GPIOE->DIR &= ~(0x05); //input
	GPIOE->DEN &= ~(0x05); 	 //Step6 : digital disable on pe2(potentiometer),pe0
	GPIOE->AMSEL |= 0x05;		 //Step7 : analog enable on pe2,pe0(0000 0101)
	SYSCTL->RCGCADC |= 0x03; //Step8 : clock enable on adc 1//checkj(0000 0011)
	usdelay(3);							 //Step9 : Delay to to enable clock
	ADC0->ACTSS &= (0x00);// sample sequensor are disabled
	ADC0->EMUX |= 0xF000;//slect the trigger event for ss3(1111 0000 0000 0000)
	ADC0->SSMUX3 |= 0x01;//sample sequencor input selectiom
	ADC0->PP |= 0x03;				 //Step10: 250ksps rate select
	ADC0->SSPRI |= 0x2000;	//Step12: 2 sequence priority(0010 0000 0000 0000)
	ADC0->SAC |= 0x08 ;//Step13: 2^4 == 16 means sample average of 16x(go upto 6 for 64x)( 0000 1000)
	ADC0->CTL |= 0x01;       //Step17: ADC_SS3_CONTROL_R |= 0x02; // Sample is end of sequence
	ADC0->SSCTL3 |= 0X02;
	ADC0->ACTSS |= (0x08);	 //turn on
	
	ADC1->ACTSS &= (0x00);// sample sequensor are disabled(ss3 ss2 ss1 ss0)=(0000 0000 0000 0000)=0x0000
	ADC1->EMUX |= 0x000F;		 // set the trigering event here i select continous triger  mode for ss0
	ADC1->SSMUX0 |= 0x03;//sample sequencor input selectiom
	ADC1->PP |= 0x03;				 //Step10: 250ksps rate select
	ADC1->SSPRI |= 0x0003;		 //Step12:  sequence priority low
	ADC1->SAC |= 0x08 ;			 //Step13: 2^4 == 16 means sample average of 16x(go upto 6 for 64x)
	ADC1->CTL |= 0x01;
	ADC1->SSCTL0 |= 0X02;
	ADC1->ACTSS |= 0x01;//turn on ss0
}
void write_LCD_Char(int data)
{
	// Interfacing details---------------
	// RS -> A2		R/W -> PA3			EN -> PA4			D0-D7 -> PB0-PB7
	GPIOB->DATA &= (0x00); // Step 1. Erase previous data
	GPIOB->DATA |= data;	 // Step 2. Pass data to 8-bit lines of LCD (D0-7)
	GPIOA->DATA &= ~(0x08);// Step 3. Enable write operation on LCD (R/W bit = 0) PA3 = 0000 1000
	GPIOA->DATA |= 0x04;	 // Step 4. Select Data Register of LCD (RS bit = 1)	PA2 = 0000 1000
	GPIOA->DATA |= 0x10;	 // Step 5. Enable LCD operation by sending high to low pulse on Enable pin	PA4 = 0001 0000
	for (j =0;j<500;j++);// Step 6. some milisecond delay to match lcd speed of writing data
	GPIOA->DATA &= ~(0x10);// Step 7. Disabled the enable for next time PA4 = 0001 0000
}

void send_LCD_Cmd(unsigned char cmd){
	// Interfacing details---------------
	// RS -> A2		R/W -> PA3			EN -> PA4			D0-D7 -> PB0-PB7
	GPIOB->DATA = cmd;		 			// Step 1. Pass command to 8-bit lines of LCD (D0-7)
	GPIOA->DATA &= ~(0x08);			// Step 2. Enable write operation on LCD (R/W bit = 0) PA3 = 0000 1000
	GPIOA->DATA &= ~(0x04);			// Step 3. Select Instructioin Register of LCD (RS bit = 0)	PA2 = 0000 0100
	GPIOA->DATA |= 0x10;	 			// Step 4. Enable LCD operation by sending high to low pulse on Enable pin 	PA4 = 0001 0000
	for (j =0; j <500 ; j++);    // Step 5. some milisecond(10us) delay to match lcd speed of writing data
	GPIOA->DATA &= ~(0x10); 	  // Step 6. Disabled for next time PA4 = 0001 0000
}
void write_LCD_Str(unsigned char *str)//Function to print String
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
 // send_LCD_Cmd(0x1C);//Step6: Shift text to right	
}

int calculateLength(int number)// Function to calculate length of the given number
	{
    int length = 0;
    if (number == 0) return 1; // Special case for 0
    while (number != 0){number /= 10;length++;}
    return length;
	}
void write_LCD_number(int number)// Function to convert an integer to its individual digits and display on LCD
	{
    int length = calculateLength(number);
    char digits[length + 1]; // Array to hold the individual digits as characters
    for (int i = length - 1; i >= 0; i--)// Extract each digit from the end
		{
        digits[i] = (number % 10) + '0'; // Convert digit to character
        number /= 10;
    }
    digits[length] = '\0'; // Null terminator
    for (int i = 0; i < length; i++){write_LCD_Char(digits[i]);}// Write each digit to the LCD
	
}
void usdelay(int time){for (i = 0; i < time*4; i++);}
void msdelay(int time1){for (i = 0; i < time1*4000; i++);}

void set_cursor(int column,int row)//sets the cursor
{
	if(column == 1){send_LCD_Cmd(0x80);}
	if(column == 2){send_LCD_Cmd(0xC0);}
	if(column == 3){send_LCD_Cmd(0x94);}
	if(column == 4){send_LCD_Cmd(0xD4);}
for(k=0 ; k < row ; k++)
{send_LCD_Cmd(0x14);}
}

void display(int adc1 , int pwm1,int adc2)
{			send_LCD_Cmd(0x01);
			usdelay(1400);// delay for 1 charachter to write is 400ms
			//set_cursor(1,0);
			//write_LCD_Str("adc1 =");
		//	set_cursor(1,12);
		 // write_LCD_number(adc1);
		//	set_cursor(2,0);
	//		write_LCD_Str("pwm_onn =");
	///		set_cursor(2,12);
		//  write_LCD_number(pwm1);
			//set_cursor(3,0);
		 // write_LCD_Str("pwm_off =");
			//set_cursor(3,12);
			//rite_LCD_number(15500-pwm1);
			//set_cursor(4,0);
			write_LCD_Str("Current =");
			//set_cursor(4,12);
			write_LCD_number(adc2);	
}

void pwm_generate(int pwm_value)
{
		GPIOE->DATA |= 0x02;// Turn PE1 High
		usdelay(pwm_value); // Delay required to set servo at 0 degrees
		GPIOE->DATA &= ~(0x02);// Turn PE1 Low
		usdelay(7500-pwm_value);//
		
}
void PC4_as_WT2CCP0_Init(void){
	SYSCTL->RCGCGPIO |= 0x04; // Step 1: Clock enable on PortC 00 0100
	for (j =0; j < 3 ; j++)	;// at least 3 clock cyles
	GPIOC->AFSEL |= 0x10;   // Step 3: Enable alternate functionality on PortB(0001 0000)
	GPIOC->DEN |= 0x10;    // Step 4: Enable digital pin functionaliy on PortB pin 2 
	GPIOC->DIR &= ~(0x10);// Step 5: Set PortB pin 2 as an output pin
	GPIOC->PCTL &= ~0x00070000;		// clear the bit fields
	GPIOC->PCTL |= 0x00070000;// Step 6: Configure PortB pin 2 as T3CCP0 pin (Table 10-2 of Datasheet, page # 651)
}
void WTIMER0A_RisingEdgeEvent_Init(void){
	SYSCTL->RCGCWTIMER |=0x01;// Step 1: Enable Timer Clock on WTIMER0 (0000 0001)
	for (j =0; j < 3 ; j++)	// wait at least 3 clock cyles
	WTIMER0->CTL = 0x00;// Step 2: Ensure Timer is disabled before making any changes TAEN = 0, i.e., WTIMER0A is disablled
	// Step 3: Select Mode of Operation of WTIMER0 (Split/cancatenated/RTC)
	WTIMER0->CFG = 0x04;// WTIMER0 is used as a 16-bit (split) timer, i.e., WTIMER0A
	WTIMER0->TAMR = 0x13;// TAMR = 3 (capture), TACMR = 0 (edge-count) TACDIR = 1 (count-up)
	// Step 4: Set counter value limit, comparted to TAR to determine match event
	WTIMER0->TAMATCHR = 0xFFFF;		// 
	WTIMER0->TAPMR = 0xFF;// used with TAMATCHR to expand to 0xFFFFFF with 8-bit prescaler (bit 16:23)
	// Step 5: Interrupt configurations
	WTIMER0->ICR = 1<<0;// Clear timer status flag (TATORIS, TATOMIS)
	// Step 6: Enable the Timer and start counting
	WTIMER0->CTL |= 1<<0;// TAEN = 1
}
int WTIMER0A_RisingEdgeEvent_Capture(void){
	return WTIMER0->TAR;	// Compared with TAMATCHR to determine match event
				// contains the number of edges that have occurred
}

// nrpm r/60s;n/60000->rounds/60ms;r/1000-> round per 1 ms
//timer off flag clear timer on
void Uart3_Init()
{
    SYSCTL->RCGCGPIO |= 0x04;  // enable clock to PORTC0 for PC6/Rx and RE5/Tx 00 0100
    SYSCTL->RCGCUART |= 0x08;  // enable clock to UART3 0000 1000
   	usdelay(1);
    //--------------------------------------------------------------------------------//
    // UART3 initialization 
    UART3->CTL   = 0;         	// UART3 module disbable 
    UART3->IBRD  = 104;      	// for 9600 baud rate, integer = 104 
    UART3->FBRD  = 11;       	// for 9600 baud rate, fractional = 11
    UART3->CC    = 0;          	//select system clock
    UART3->LCRH  = 0x60;     	// data lenght 8-bit, not parity bit, no FIFO 
    UART3->CTL   = 0x301;     	// Enable UART3 module, Rx and Tx 
    GPIOC->DEN   = 0xC0;      	// set PC6 and PC7 as digital (1100 0000) 
    GPIOC->AFSEL = 0xC0;    	// Use PC6,PC7 alternate function (1100 0000)
    GPIOC->AMSEL = 0;    		// Turn off analg function
    GPIOC->PCTL  = 0x11000000; 	// configure PC6 and PC7 for UART 
	usdelay(1); 
}
void UART3_Transmitter(char data1)  
{
    while((UART3->FR & 0x20) != 0); /* wait until Tx buffer not full */
    UART3->DR = data1;                  /* before giving it another byte */
}