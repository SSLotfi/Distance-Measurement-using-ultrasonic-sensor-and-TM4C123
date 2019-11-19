
#include "TM4C123.h"                    // Device header
#include <stdint.h>
#include "inttypes.h"
#include "C:/Keil/Labware/tm4c123gh6pm.h" 
void delay_Microsecond(uint32_t time);
void Timer0_init(void);
uint32_t measureD(void);
void portC_init(void)
 {
	uint32_t delay ;
	SYSCTL_RCGCGPIO_R |= 0x04;
	delay= 1;
	GPIO_PORTC_LOCK_R = 0x4C4F434B;      
  GPIO_PORTC_DIR_R   |=  0xF0;     
  GPIO_PORTC_DEN_R  |=  0xF0;   
  GPIO_PORTC_CR_R  =  0xF0;	
	

 }
 void portB_init(void) 
 {
	uint32_t delay ;
	SYSCTL_RCGCGPIO_R |= 0x02;
	delay= 1;
	GPIO_PORTB_LOCK_R = 0x4C4F434B;
  GPIO_PORTB_PCTL_R  =  0;       
  GPIO_PORTB_DIR_R   =  0xFF;   
  GPIO_PORTB_PUR_R  =  0;  
  GPIO_PORTB_DEN_R  =  0xFF;   
  GPIO_PORTB_CR_R  =  0xFF;	
  GPIO_PORTB_AMSEL_R = 0;	
	GPIO_PORTB_AFSEL_R = 0;
 }
  
 void portD_init(void)
 {
	uint32_t delay ;
	SYSCTL_RCGCGPIO_R |= 0x08;
	delay= 1;
	GPIO_PORTD_LOCK_R = 0x4C4F434B;
  GPIO_PORTD_PCTL_R  =  0;       
  GPIO_PORTD_DIR_R   =  0x3F;   
  GPIO_PORTD_PUR_R  =  0;  
  GPIO_PORTD_DEN_R  =  0x3F;   
  GPIO_PORTD_CR_R  =  0x3F;	
  GPIO_PORTD_AMSEL_R = 0;	
	GPIO_PORTD_AFSEL_R = 0;
 }
  void portE_init(void)
 {
	uint32_t delay ;
	SYSCTL_RCGCGPIO_R |= 0x10;
	delay= 1;
	GPIO_PORTE_LOCK_R = 0x4C4F434B;
  GPIO_PORTE_PCTL_R  =  0;       
  GPIO_PORTE_DIR_R   =  0xFF;   
  GPIO_PORTE_PUR_R  =  0;  
  GPIO_PORTE_DEN_R  =  0xFF;   
  GPIO_PORTE_CR_R  =  0xFF;	
  GPIO_PORTE_AMSEL_R = 0;	
	GPIO_PORTE_AFSEL_R = 0;
 }
const double _16MHz_1clock = 62.5e-9; /*Value of 1clock cycle in nanoseconds*/
const uint32_t MULTIPLIER  = 5882;  /*Derived from speed of sound*/
#define ECHO (1U<<6) //PB6
#define TRIGGER (1U<<4) //PA4(OUTPUT)
#define BLUE_LED 4 //(1U<<2)//PF3 onboard Blue LED 
uint32_t highEdge,lowEdge;
uint32_t ddistance; /*Distance in centimeters*/
uint32_t counter =0; 
uint32_t measureD(void) { 
GPIOA->DATA &=~TRIGGER;
    delay_Microsecond(12);
    GPIOA->DATA |= TRIGGER;
    delay_Microsecond(12);
    GPIOA->DATA &=~TRIGGER;
    /*Capture firstEgde i.e. rising edge*/
    TIMER0->ICR =4;
    while((TIMER0->RIS & 4)==0){}; //Wait till captured
        highEdge =  TIMER0->TAR;
 
        /*Capture secondEdge i.e. falling edge */
        TIMER0->ICR =4; //clear timer capture flag
        while((TIMER0->RIS & 4)  ==0){};
            lowEdge = TIMER0->TAR;
              ddistance = lowEdge -highEdge;
              ddistance = _16MHz_1clock *(double) MULTIPLIER *(double)ddistance;
 
            return ddistance;
 
}
void divide(uint32_t x){
	uint32_t y ;
	uint32_t z ; 
	uint32_t i;
	y = x%10; // 8
	x = x/10;
	z = x%10; // 2
	z = z<<4;
	x = x/10;
	i = x%10; // 1
	i = i<<1;
	GPIO_PORTD_DATA_R = 0;
  GPIO_PORTE_DATA_R = 0;

  GPIO_PORTD_DATA_R = y; //D0-D3 //unit
	GPIO_PORTC_DATA_R = z;  //ten c1-c4
	GPIO_PORTE_DATA_R = i; //E1-E4 //hundred
	
	
	
	}

int main(void){
    SYSCTL->RCGCGPIO |=(1U<<0); //Enable clock for PORTA 
        SYSCTL->RCGCGPIO |=(1U<<5); //Enable clock for PORTF 
        GPIOA->DIR =TRIGGER;  
    GPIOF->DIR =BLUE_LED;
    GPIOA->DEN |=(ECHO)|(TRIGGER);
    GPIOF->DEN |= BLUE_LED;
	portD_init();
	portE_init();
	portC_init();
    while(1){
        Timer0_init();
         divide(measureD());
    if(measureD() >40) 
   GPIOF->DATA |=BLUE_LED;
   else
   GPIOF->DATA &=~BLUE_LED;
   delay_Microsecond(1000000);
 
  }
}
void delay_Microsecond(uint32_t time)
{
    int i;
    SYSCTL->RCGCTIMER |=(1U<<1); 
        TIMER1->CTL=0;
    TIMER1->CFG=0x04;
    TIMER1->TAMR=0x02;
    TIMER1->TAILR= 16-1;
    TIMER1->ICR =0x1;
    TIMER1->CTL |=0x01;
 
    for(i=0;i<time;i++){ 
       while((TIMER1->RIS & 0x1)==0);
        TIMER1->ICR = 0x1;
    }
 
}
void Timer0_init(void)
{
    SYSCTL->RCGCTIMER |=(1U<<0); 
        SYSCTL->RCGCGPIO |=(1U<<1); 
        GPIOB->DIR &=~ECHO;
    GPIOB->DEN |=ECHO;
    GPIOB->AFSEL |=ECHO;
    GPIOB->PCTL &=~0x0F000000;
    GPIOB->PCTL |= 0x07000000;
 
    TIMER0->CTL &=~1;
    TIMER0->CFG =4;
    TIMER0->TAMR = 0x17;
    TIMER0->CTL |=0x0C;
    TIMER0->CTL |=1;
}
