//|_______________________________________________________________________________________________________________________________________________________|
//|Task 3.3: Interface and program an LED connected at Port B Pin XX to toggle at 1/4 Hz with 75% duty cycle (where XX = Roll Number%8).								  |
//|-------------------------------------------------------------------------------------------------------------------------------------------------------|
//|Name : Haseeb-ul-hassan																																																																|
//|Date : 25-FEB-2024																																																																			|
//|Hardware: Tiva C series Launchpad Evaluation kit by Texas instruments																																									|	
//|IDE and Compiler : Keil microvision 5.39																																																								|
//|Disclaimer : This program has been written as an open source develpment program so any usage must be credited and i am not responsible for any missuse |
//|Contact : 2022mc58@student.uet.edu.pk(feel free to ask me anything about this code	)																																		|
//|_______________________________________________________________________________________________________________________________________________________|
// Roll no is 58 hence pin2



//_________________________________________________________________________________________________________________________________________________________
// Configuring Desired  Register Adresses... 
//_________________________________________________________________________________________________________________________________________________________
#define RCGC_GPIO_R *(( volatile unsigned long *)0x400FE608) 					 //Configuration Registers for Registor Clock Gatting Control(RCGC)
// PortB on ADvance Periferal Bus(APB) = 0x4000 5000 (Base Address)		// Port B Base Register Adress(Main Adress)
#define GPIO_PortB_AFSEL_R *(( volatile unsigned long *)0x40005420)  // Alternative Function Select Register Adress(Offset 0x420)
#define GPIO_PortB_DEN_R *(( volatile unsigned long *)0x4000551C)   // Digital Enable Register Adress(Offset 0x51C)
#define GPIO_PortB_DIR_R *(( volatile unsigned long *)0x40005400)	 // Direction Control Register Adress(Offset 0x400)
#define GPIO_PortB_DATA_R *(( volatile unsigned long *)0x40005010)//Digital Data Rigister Adress(Determined using IDK)
//_________________________________________________________________________________________________________________________________________________________
// Assigning Registers Values...
//_________________________________________________________________________________________________________________________________________________________
#define GPIO_PortB_CLOCK_EN 0x02  //(0000 0010) PortB clock gating control
#define GPIO_PortB_AFSEL_DS 0x00 //(0000 0000)Port B alternate function disable
#define GPIO_PortB_pin2_EN 0x04 //(0000 0100) Port B pin 2 digital enable
#define GPIO_PortB_pin2_OP 0x04//(0000 0100) Port B pin 2 as digital output
#define GPIO_PortB_pin2 0x04  //(0000 0100) Mask for pin 2 write operation 
//_________________________________________________________________________________________________________________________________________________________
//Setting System Clock...
//_________________________________________________________________________________________________________________________________________________________
#define SYSTEM_CLOCK_FREQUENCY 16000000 // Default clock frequency
#define DELAY_VALUE 334000
//_________________________________________________________________________________________________________________________________________________________
//Mian Program ...
//_________________________________________________________________________________________________________________________________________________________
unsigned volatile long j;
int main ()
{
	RCGC_GPIO_R |= GPIO_PortB_CLOCK_EN;// Step 1: Clock enable on PortB
	for (j =0; j < 3 ; j++)						// at least 3 clock cyles
																	 // Step 2: APB is selected for PortB by selecting 0x40005000 as Base Address in DATA section
	GPIO_PortB_AFSEL_R &= GPIO_PortB_AFSEL_DS;// Step 3: Disable alternate functionality on PortB
	GPIO_PortB_DEN_R |= GPIO_PortB_pin2_EN;  // Digital enable for PB2
	GPIO_PortB_DIR_R |= GPIO_PortB_pin2_OP; // PB2 as output
	while (1)															 // Loop to run infinite
	{GPIO_PortB_DATA_R ^= GPIO_PortB_pin2;// Toggle the Pin2
		for (j =0; j< DELAY_VALUE ; j++);  // delay loop

	}
}//_________________________________________________________________________________________________________________________________________________________
//If You find a gun in the last comments row then shot the Tiva with this Code
//_______________________________________________________(HAPPY-CODDING)___________________________________________________________________________________