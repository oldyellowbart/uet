//c4
#include "TM4C123.h"                    // Device header
unsigned volatile long j;
uint32_t edge_count = 0;
void PD0_as_WT2CCP0_Init(void);
void WTIMER0A_RisingEdgeEvent_Init(void);
int WTIMER0A_RisingEdgeEvent_Capture(void);
int main()
{
	
	PD0_as_WT2CCP0_Init();// Initialize PortD pin 2 as T3CCP0
	WTIMER0A_RisingEdgeEvent_Init();// Initialize WTIMER0A as Rising Edge Event Counter
	
	while(1)
    {
	edge_count = WTIMER0A_RisingEdgeEvent_Capture();
	GPIOB->DATA = CC_7seg_Table[edge_count];
	}
}
void PD0_as_WT2CCP0_Init(void){
	SYSCTL->RCGCGPIO |= 0x04; // Step 1: Clock enable on PortD 00 0100
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

// Configure the system clock to 16 MHz using PLL
void PLL_Init(void){

  // 0) Use RCC2
  SYSCTL->RCC2 |=  0x80000000;  // USERCC2

  // 1) bypass PLL while initializing
  SYSCTL->RCC2 |=  0x00000800;  // BYPASS2, PLL bypass

  // 2) select the crystal value and oscillator source
  SYSCTL->RCC = (SYSCTL->RCC &~ 0x000007C0) + 0x00000540;  // clear XTAL field, bits 10-6  // 10101, configure for 16 MHz crystal
  SYSCTL->RCC2 &= ~0x00000070;  // configure for main oscillator source

  // 3) activate PLL2 by clearing PWRDN
  SYSCTL->RCC2 &= ~0x00002000;

  // 4) set the desired system divider
  SYSCTL->RCC2 |= 0x40000000;   // use 400 MHz PLL
  SYSCTL->RCC2 = (SYSCTL->RCC2 &~ 0x1FC00000) + (24<<22); 15.38     // configure for 16 MHz clock / / clear system clock divider +

  // 5) wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL->RIS&0x00000040)==0){}  // wait for PLLRIS bit

  // 6) enable use of PLL by clearing BYPASS
  SYSCTL->RCC2 &= ~0x00000800;
}