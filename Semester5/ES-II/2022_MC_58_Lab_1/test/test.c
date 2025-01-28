/* 
-----------------------------------------------------------------------
** Interfacing details---------
RS   -> PA2
R/W  -> PA3
EN   -> PA4
D0-7 -> PB0-PB7
PE2  -> ADC0_in 0-3.3v 0-4096
PWM  -> PF2
PF2  -> pwm generation 
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
int timericr;
int iklm,sm;
float current ,th_rpm,adc_data,adc_data2,pwm,pwm2,err,factor,sample;

uint32_t edge_count = 0;
unsigned volatile long i;
unsigned volatile long j;
unsigned volatile long k;
//unsigned volatile long err;
// int  = 0;
//unsigned int adc_data2 = 0;
//unsigned volatile long  =0;
//unsigned volatile long  =0;
//---------------------------------------------------------------------------//
// Function Declarations
void PF2_as_M1PWM6_Init(void);
void PWM_Module1_Channel6_Init(void);
void Port_A_2_3_4_as_Output_Init(void);
void PortB_as_Output_Init(void);
void PLL_Init(void);
void PortE_as_analog_Input_Init(void);
void write_LCD_Char(int data);
void write_LCD_number(float number);
void send_LCD_Cmd(unsigned char cmd);
void write_LCD_Str(unsigned char *str);
char UART5_Receiver(void);
void UART5_Transmitter(char data);
void LCD_Init(void);
void Uart5_Init(void);
void set_cursor(int column,int row);
void display(float adc1 , float pwm1,float edge,float current);
void msdelay(int time1);
void usdelay(int time);
void PC4_as_WT2CCP0_Init(void);
void WTIMER2A_1s_Init(void);
void WTIMER0A_RisingEdgeEvent_Init(void);
int calculateLength(int number);
void write_UART_number(int number);
void write_uart_Str(unsigned char *str1);
void Uart_Graph(int rpm1, int rpm2, int current);
//==================================================>(Main Function Start)<========================================================_// 

int main()
	{
		Port_A_2_3_4_as_Output_Init();
		PortB_as_Output_Init();
		PC4_as_WT2CCP0_Init();// Initialize PortD pin 2 as T3CCP0
		PortE_as_analog_Input_Init();
		PF2_as_M1PWM6_Init();
		WTIMER0A_RisingEdgeEvent_Init();// Initialize WTIMER2A as Rising Edge Event Counter
		WTIMER2A_1s_Init();
		PWM_Module1_Channel6_Init();
		Uart5_Init();
		LCD_Init();
		PLL_Init();
	while(1)
		{	
		factor = (adc_data/4000)*edge_count;// factor 
		th_rpm = ((adc_data/4000)*930*12);
		adc_data = ( ADC0->SSFIFO3 & 0xFFF);//pwm
		if(adc_data <350){adc_data =0;};
		pwm = ((19000-(adc_data * 0.25)));
		err =((th_rpm-factor)*100)/th_rpm;
		
		for(int op =0;op<999;op++)
		{
			if ((ADC1->ACTSS & (0<<16)) == 0){adc_data2 = ( ADC1->SSFIFO0 & 0xFFF);}//check conversion completeionn
		current = (((adc_data2-3110))*13/1000)-0.05; //convert to cuurent
		sample = sample + current;
		}
		
		current = sample/1000;
			sample = 0;
		PWM1->_3_CMPA = pwm;	
			
		Uart_Graph(th_rpm,factor,current*1000);
		msdelay(10);
	}
	}
	
	//==================================================>(Main Function End)<========================================================_// 
	
	
	
	//-------------------------------------------->(Function Defintions)<---------------------------------------------//
	
	//-------------------------------- Configure the system clock to 16 MHz using PLL-------------------------------
	void PLL_Init(void)
{
  SYSCTL->RCC2 	|=  0x80000000;   				// Step 1: Use RCC2 to cofigure RCC
  SYSCTL->RCC2 	|=  0x00000800;   				// Step 2: bypass PLL while initializing  
  SYSCTL->RCC  	 =  (SYSCTL->RCC &~ 0x000007C0) + 0x00000540; // Step 3:  clear XTAL field, bits 10-6  10101, configure for 16 MHz crystal
  SYSCTL->RCC2 	&=  ~0x00000070;  				// Step 4: configure for main oscillator source
  SYSCTL->RCC2 	&=  ~0x00002000;  				// Step 5: activate PLL by clearing PWRDN
  SYSCTL->RCC2 	|=  0x40000000;   				// Step 6: set the desired system divideruse 400 MHz PLL
  SYSCTL->RCC2 	 =  (SYSCTL->RCC2 &~ 0x1FC00000) + (24<<22);     // Step 7:configure for 16 MHz clock  clear system clock divider +
  while((SYSCTL->RIS&0x00000040)==0){} 		// Step 8: wait for the PLL to lock by polling PLLLRIS // wait for PLLRIS bit
  SYSCTL->RCC2 	&=  ~0x00000800;  				// Step 9: enable use of PLL by clearing BYPASS
}

//-----------------------------------------------(Port_A Intialization)-----------------------------------------------

void Port_A_2_3_4_as_Output_Init(void)
{
	SYSCTL->RCGCGPIO 	 |= 0x01;							// Step 1: Clock enable on PortA		 															(00 0001)
	usdelay(1);	   													// Step 2: wait at least 3 clock cyle
	GPIOA->AFSEL			 &= (0x1C); 					// Step 3: Disable alternate functionality on PortA 						(0000 1100)
	GPIOA->DEN 				 |= 0x1C; 						// Step 4: Enable digital pin  on PortA pin 2,3,4								(0001 1100)
	GPIOA->DIR 				 |= 0x1C;							// Step 5: Set PortA pin 0-1 as an output pin										(0000 1100)
}

//-----------------------------------------------(Port_B Intialization)-----------------------------------------------

void PortB_as_Output_Init(void)
{
	SYSCTL->RCGCGPIO 		|= 0x02;						// Step 1: Clock enable on PortB 																(00 0010)
	usdelay(1);						   								// Step 2: wait at least 3 clock cyle
	GPIOB->AFSEL 		 		&= (0x00); 					// Step 3: Disable alternate functionality on PortB 						(0000 0000)
	GPIOB->DEN 			 		|= 0xFF;   					// Step 4: Enable digital pin functionaliy on PortB pin 0-7			(1111 1111)
	GPIOB->DIR 			 		|= 0xFF;						// Step 5: Set PortB pin 0-7 as an output pin										(1111 1111)
}

//-----------------------------------------------(Port_C Intialization)-----------------------------------------------

void PC4_as_WT2CCP0_Init(void){
	SYSCTL->RCGCGPIO 		|= 0x04; 						// Step 1: Clock enable on PortC 																	(00 0100)
	usdelay(1);															// Step 2: Wait at least 3 clock cyles
	GPIOC->AFSEL		 		|= 0x10;   					// Step 3: Enable alternate functionality on PortC4							(0001 0000)
	GPIOC->DEN 			 		|= 0x10;    				// Step 4: Enable digital pin on PortC pin 4 										(0001 0000)
	GPIOC->DIR 			 		&= ~(0x10);					// Step 5: Set PortB pin 2 as an output pin           					(0001 0000)
	GPIOC->PCTL 		 		&= ~0x00070000;			// Step 6: Clear the bit fields
	GPIOC->PCTL 		 		|= 0x00070000;			// Step 7: Configure PortB pin 2 as T3CCP0 pin (Table 10-2 of Datasheet, page # 651)
}

//-----------------------------------------------(Port_E Intialization)-----------------------------------------------

void PortE_as_analog_Input_Init(void)
{
	
	SYSCTL->RCGCGPIO 		|= 0x10;						// Step1  : Enable the clock for Port E 													(01 0000)
	usdelay(1);															// Step2  : Delay 4 clock cycle to enable clock 
	GPIOE->AFSEL 				|= (0x05);					// Step3  : Enable alternate on pe0,pe2 												(0000 0101)
	GPIOE->AMSEL 				&= ~(0x02); 				// Step4  : Disable_analog_mode_select_on PE1										(1111 1101)
	GPIOE->DEN 					|= 0x02;						// Step5  : Enable digital pin functionaliy											(0000 0010)
	GPIOE->DIR 					|= 0x02;						// Step6  : PE1 digital pins as an output 											(0000 0010)
	GPIOE->DIR					&= ~(0x05); 				// Step7  : PE0,PE1 as input 																		(0000 0101)
	GPIOE->DEN 					&= ~(0x05); 				// Step8  : digital disable on pe2,pe0													(0000 0101)
	GPIOE->AMSEL 				|= 0x05;						// Step9  : analog enable on pe2,pe0														(0000 0101)
	SYSCTL->RCGCADC			|= 0x03;	 					// Step10 : clock enable on adc 0,1 														(0000 0011)
	usdelay(1);															// Step11 : Delay to to enable clock
	
	//-------------------------------------------------> ADC_0 <------------------------------------------------------//
	
	ADC0->ACTSS 				&= (0x00);					// Step12 : sample sequensor are disabled
	ADC0->EMUX 					|= 0xF000;					// Step13 :	slect the trigger event for ss3						(1111 0000 0000 0000)
	ADC0->SSMUX3 				|= 0x01;						// Step14 :	sample sequencor input selectiom
	ADC0->PP 						|= 0x03;						// Step15	: 250ksps rate select
	ADC0->SSPRI 				|= 0x2000;					// Step16	: 2 sequence priority												(0010 0000 0000 0000)
	ADC0->SAC 					|= 0x07  ;					// Step17	: 2^4->16 sample average of 16x(upto 6 for 64x)			 ( 0000 1000)
	ADC0->CTL 					|= 0x01;    				// Step18	: Voltage refrence reserved														(0000 0001)
	ADC0->SSCTL3 				|= 0x06;						// Step19	: ADC sample sequenser3 control->end of seq						(0000 0010)
	ADC0->ACTSS 				|= 0x08;						// Step20	: turn on SS3(Sample Sequencer 3)
	
	//-------------------------------------------------> ADC_1 <-------------------------------------------------------//
	
	ADC1->ACTSS 				&= (0x00);					// Step21	: SS disabled(ss3 ss2 ss1 ss0)							(0000 0000 0000 0000)
	ADC1->EMUX 					|= 0x000F;					// Step22	: select continous triger  mode for ss0			(0000 0000 0000 1111)
	ADC1->SSMUX0 				|= 0x03;						// Step23	: sample sequencor input selectiom										(0000 0011)
	ADC1->PP 						|= 0x03;						// Step24 : 250ksps rate select																	(0000 0011)
	ADC1->SSPRI 				|= 0x0003;					// Step25 : sequence priority low											(0000 0000 0000 0011)
	ADC1->SAC 					|= 0x07 ;						// Step26 : 2^4->16  sample average of 16x(upto 6 for 64x)			(0000 1000)
	ADC1->CTL 					|= 0x01;						// Step27	: Voltage refrence reserved														(0000 0001)
	ADC1->SSCTL0 				|= 0X02;						// Step28	:	ADC sample sequenser0 control ->end of seq					(0000 0010)		
	ADC1->ACTSS 				|= 0x01;						// Step29	:	turn on ss0
}
//--------------------------------------------------(Port_F Intialization)-----------------------------------------------

void PF2_as_M1PWM6_Init(void)
	{
	SYSCTL->RCGCGPIO 		|= 0x20;						// Step 1: Clock enable on PortF																(0010 0000) 
	usdelay(1);															// Step 2: at least 3 clock cyles
	GPIOF->AFSEL 		 		|= 1<<2;						// Step 3: Enable alternate functionality on PortF							(0000 0100)
	GPIOF->DEN			 		|= 1<<2;						// Step 4: Digital enable for PF2																(0000 0100)
	GPIOF->DIR 			 		|= 1<<2; 						// Step 5: PF2 as Output pin																		(0000 0100)
	GPIOF->PCTL 		 		&= 0xFFFFF0FF;			// Step 6: Clear the bit fields																	(FFFF F0FF)
	GPIOF->PCTL			 		|= 0x00000500;			// Step 7: PortF pin 2 as M1PWM6 pin (Table 10-2 of  Datasheet, page # 651)
}
	

void write_LCD_Char(int data)
{
	// -----------------------------------------------Interfacing details-----------------------------------------------
	
	// RS -> A2		R/W -> PA3			EN -> PA4			D0-D7 -> PB0-PB7
	GPIOB->DATA 				&= (0x00);  				// Step 1. Erase previous data																	(0000 0000)
	GPIOB->DATA 				|= data;						// Step 2. Pass data to 8-bit lines of LCD (D0-7)								(		DATA	)
	GPIOA->DATA 				&= ~(0x08); 				// Step 3. Enable write operation on LCD (R/W bit = 0)PA3   		(1111 0111)
	GPIOA->DATA 				|= 0x04;						// Step 4. Select Data Register of LCD (RS bit = 1)	PA2 = 8			(0000 1000)
	GPIOA->DATA 				|= 0x10;						// Step 5. Enable LCD by sending high to low pulse on En-pin		(0001 0000)
	for (j =0; j <500 ; j++);										// Step 6. milisecond delay to match lcd speed of writing data	(__delay__)
	GPIOA->DATA 				&= ~(0x10);					// Step 7. Disabled the enable for next time PA4 = 							(0001 0000)
}

void send_LCD_Cmd(unsigned char cmd)
{
	// ----------------------------------------------Interfacing details-------------------------------------------------
	
	// RS -> A2		R/W -> PA3			EN -> PA4			D0-D7 -> PB0-PB7
	GPIOB->DATA 			  = cmd;		 					// Step 1. Pass command to 8-bit lines of LCD (D0-7)						(		CMD		)
	GPIOA->DATA 				&= ~(0x08);					// Step 2. Enable write operation on LCD (R/W bit = 0)  				(0000 1000)
	GPIOA->DATA 				&= ~(0x04);					// Step 3. Select Instructioin Register of LCD (RS bit = 0)			(0000 0100)
	GPIOA->DATA 				|= 0x10;	 					// Step 4. Enable sending high to low pulse on Enable pin 			(0001 0000)
	for (j =0; j <500 ; j++); 							// Step 5. some milisecond(10us) delay to match lcd speed of writing data
	GPIOA->DATA 				&= ~(0x10); 				// Step 6. Disabled for next time PA4 													(0001 0000)
}
//-------------------------------------------------(Optocoupler_intialize)-----------------------------------------------

void WTIMER0A_RisingEdgeEvent_Init(void)
{
	SYSCTL->RCGCWTIMER 	|=0x01;							// Step 1: Enable WTIMER0 clock																	(0000 0001)
	usdelay(1);															// Step 2: wait at least 3 clock cyles													(__delay__)
	WTIMER0->CTL 				 = 0x00;						// Step 3: Timer is disabled to make changes TAEN = 0, i.e.			(0000 0000)
	WTIMER0->CFG 				 = 0x04;						// Step 4: WTIMER0 is used as a 16-bit (split) timer, i.e.,			(0000 0100)
	WTIMER0->TAMR 			 = 0x13;						// Step 5: TAMR = 3,TACMR = 0,TACDIR = 1(capt,edge-cnt,cnt-up)	(0001 0011)	
	WTIMER0->TAMATCHR 	 = 0xFFFF;					// Step 6: Set counter value limit															(FFFF FFFF)
	WTIMER0->TAPMR 			 = 0xFF;						// Step 7: use to expand to 8-bit prescaler (bit 16:23)					(0000 00FF)	
	WTIMER0->ICR 				 = 1<<0;						// Step 8: Interrupt Clear timer status flag (TATORIS, TATOMIS)	(0000 0001)
	WTIMER0->CTL 				|= 1<<0;						// Step 9: TAEN = 1 Enable the Timer and start counting					(0000 0001)
}
//-------------------------------------------------(I-second interrupt)-----------------------------------------------

void WTIMER2A_1s_Init(void) 
{								//   Opto-coupler  interrupt 
	SYSCTL->RCGCWTIMER 	|= 1 << 2;					// Step 1: Enable WTIMER2 clock																	(0000 0010)
	usdelay(1); 														// Step 2: Wait at least 3 clock cycles													(__delay__)
	WTIMER2->CTL 				 = 0x00; 						// Step 3: TAEN = 0, i.e., WTIMER2A is disabled									(0000 0000)
	WTIMER2->CFG 				 = 0x4; 						// Step 4: WTIMER2 is used as a 32-bit (split) timer						(0000 0100)
	WTIMER2->TAMR 			 = 0x2 ; 						// Step 5: TAMR = 2,TACMR = 0,TACDIR = 0,(capt,edge-cnt,cnt-dn)	(0000 0010)
	WTIMER2->TAILR 			 = 16000000-1; 			// Step 6: Set the timer period to 1 second											(016000000)
	WTIMER2->ICR 				&= ~(1 << 0); 			// Step 7: Clear timer status flag (TATORIS, TATOMIS)						(xxxx xxx0)
	WTIMER2->IMR				|= 1 << 0;	 				// Step 8: Enable WTIMER2A timeout interrupt										(0000 0001)
	WTIMER2->CTL 			 	|= 0x01; 						// Step 9: Enable the WTIMER2A TAEN = 1													(0000 0001)
	NVIC->ISER[3] 		  |= (1<<2); 					// Step 10: Enable the WTIMER2A interrupt in the NVIC				
//	NVIC->IP[5] = 0x00002000 ;  					// Step 11: Set interrupt priority 2
}
//---------------------------------------------------(Uart Intialization)-----------------------------------------------

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
//---------------------------------------------------(PWM_Intialization)-----------------------------------------------

void PWM_Module1_Channel6_Init(void)
{	
	SYSCTL->RCGCPWM 		|= (1<<1);					// Step 1: Clock Gating Control of PWM Module 1 								(0000 0010)
	for (j =0; j < 10 ; j++)								// Step 2: at least 3 clock cyles																(__delay__)
	SYSCTL->RCC			 		 = 0x00160000;			// Step 3: enable signal divisor for PWM divided by 16 				 (0x00160000)
	PWM1->_3_CTL 		 		 = 0x00;						// Step 4: Disable Generator 3 before config										(0000 0000)
	PWM1->_3_LOAD 	 		 = 20000;						// Step 5: Set Load value for 10 kHz(1Mhz / 50Hz = 20000)						(20000)
	PWM1->_3_CMPA 			 = 19000;						// Step 6: set duty cycle 5% of Load value 18000-19000							(19000)
	PWM1->_3_GENA 			|= 0x8C;						// Step 7: Perform Generating Control Register configuration PWM signal HIGH when counter relaoded and LOW when matches CMPA Value
	PWM1->_3_CTL 				|= 0x01;						// Step 8: Enable generator 3 counter														(0000 0001)
	PWM1->ENABLE 				|= 1<<6;						// Step 9: Enalbe PWM Module 1 Channel 6 Output									(0010 0000)
}
//-----------------------------------------------------(Function to set lcd)------------------------------------------------//

void LCD_Init(void)
{
	send_LCD_Cmd(0x38);											//Step1: Function Set: 8-bit, 2 Line, 5x7 Dots									(0011 1000)
	send_LCD_Cmd(0x06);											//Step2: Auto increment cursor (shift cursor to right)					(0000 0110)
	send_LCD_Cmd(0x0C);											//Step3: Display on, cursor off																	(0000 1100)
	send_LCD_Cmd(0x01);											//Step4: Clear display screen																		(0000 0001)
	send_LCD_Cmd(0x80);											//Step5: Force cursor to the beginning (1st line)								(1000 0000)
}

//---------------------------------------(Function to calculate length of the given number)----------------------------------------

int calculateLength(int number)
	{
    int length = 0;
    if (number == 0) return 1; 						// Special case for 0
    while (number != 0){number /= 10;length++;}
    return length;
	}
	
//---------------------------------(Function to convert an integer to its individual digits and display on LCD)-------------------------
	
void write_LCD_number(float number)
{
    int integer_part = (int)number;  								// Get the integer part
    float fractional_part = number - integer_part;  // Get the fractional part
    int length = calculateLength(integer_part);			// First, display the integer part (same logic as before)
    char digits[length + 1];  											// Array to hold the digits as characters
    for (int i = length - 1; i >= 0; i--) {
        digits[i] = (integer_part % 10) + '0';  		// Convert integer to char
        integer_part /= 10;
    }
    
    digits[length] = '\0';  // Null terminator
    
    // Write integer digits to LCD
    for (int a = 0; a < length; a++) {
        write_LCD_Char(digits[a]);
    }
    
    // Add a decimal point
    write_LCD_Char('.');
    
    // Handle the fractional part (assumes 2 decimal places for simplicity)
    fractional_part *= 100;  // Scale the fractional part to an integer (for 2 decimal places)
    int frac_part_int = (int)fractional_part;
    
    // Display fractional digits
    write_LCD_Char((frac_part_int / 10) + '0');  // Tens place of the fractional part
    write_LCD_Char((frac_part_int % 10) + '0');  // Ones place of the fractional part
}

void usdelay(int time){for (i = 0; i < time*4; i++);}
void msdelay(int time1){for (i = 0; i < time1*4000; i++);}//1358

void set_cursor(int row,int column)				//sets the cursor line & position
{
	if(row == 1){send_LCD_Cmd(0x80);}				// switch to line 1 
	if(row == 2){send_LCD_Cmd(0xC0);}				// switch to line 2
	if(row == 3){send_LCD_Cmd(0x94);}				// switch to line 3
	if(row == 4){send_LCD_Cmd(0xD4);}				// switch to line 4
for(k=0 ; k < column ; k++)				 				//	Set cursor position
{
	send_LCD_Cmd(0x14);}						 				// Shift cursor to write 
}

void display(float adc1 , float pwm1,float edge,float current)
{			
			send_LCD_Cmd(0x01);
			usdelay(1400);// delay for 1 charachter to write is 400ms
			set_cursor(1,0);
			write_LCD_Str("error %");
			set_cursor(1,12);
			write_LCD_number(adc1);
			set_cursor(2,0);
			write_LCD_Str("Th_rpm =");
			set_cursor(2,12);
		  write_LCD_number(pwm1);
			set_cursor(3,0);
			write_LCD_Str("RPM =");
			set_cursor(3,12);
			write_LCD_number(edge);
			set_cursor(4,0);
			write_LCD_Str("Current =");
			set_cursor(4,12);
			write_LCD_number(current);	
}

void UART5_Transmitter(char data)  
{
  while((UART5->FR & 0x20) != 0); // wait until Tx buffer not full 
  UART5->DR |= data;              // before giving it another byte 
}

void WTIMER2A_Handler(void) 			//   Opto-coupler   interrupt handler
{										
	edge_count = (WTIMER0->TAR)*30;
  WTIMER0->TAV = 0; 							// Reset Timer0 value
	display(err,th_rpm,factor,current);
	WTIMER2->ICR |= (1 << 0);				// Clear the WTIMER2A interrupt flag
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
	void write_LCD_Str(unsigned char *str)
	{
		unsigned int i, len;
		len = strlen(str);
		for (i = 0; i < len; i++)
		{
		write_LCD_Char(str[i]);
		}
	}

	void Uart_Graph(int theo_rpm,int prac_rpm,int CUrrent)
{
		write_UART_number(theo_rpm);//ch1
		UART5_Transmitter(',');
		write_UART_number(prac_rpm);//ch2
		UART5_Transmitter(',');
		write_UART_number(CUrrent);//ch3
		UART5_Transmitter('\r\n');//next line
}