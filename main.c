#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"
//
char readChar(void);
void printChar(char c);
void printString(char * string);
char* readString(char delimiter);

int main(void) 
{
  
  char c ;
    //  Enable the UART module using the RCGCUART register.
    SYSCTL_RCGCUART_R |= (1<<0); 
    
    //  Enable the clock to the appropriate GPIO module via the RCGCGPIO register.
    SYSCTL_RCGCGPIO_R |= (1<<0); 
    
    //  Set the GPIO AFSEL bits for the appropriate pins.
    GPIO_PORTA_AFSEL_R = (1<<1)|(1<<0); 
    
    
    //  Configure the PMCn fields in the GPIOPCTL register to assign the UART signals to the appropriate
    // pins.
    GPIO_PORTA_PCTL_R = (1<<0)|(1<<4);  
    
    GPIO_PORTA_DEN_R = (1<<0)|(1<<1); 
    
    // Find  the Baud-Rate Divisor
    // BRD = 16,000,000 / (16 * 9600) = 104.16666666666666666666666666666666666666666666666666
    // UARTFBRD[DIVFRAC] = integer(0.166667 * 64 + 0.5) = 11
    
    
    // With the BRD values in hand, the UART configuration is written to the module in the following order
                   
    // 1. Disable the UART by clearing the UARTEN bit in the UARTCTL register
    UART0_CTL_R &= ~(1<<0);
    
    // 2. Write the integer portion of the BRD to the UARTIBRD register
    UART0_IBRD_R = 104;      
    // 3. Write the fractional portion of the BRD to the UARTFBRD register.
    UART0_FBRD_R = 11; 
    
    // 4. Write the desired serial parameters to the UARTLCRH register (in this case, a value of 0x0000.0060)
    UART0_LCRH_R = (0x3<<5)|(1<<4);     // 8-bit, no parity, 1-stop bit
    
    // 5. Configure the UART clock source by writing to the UARTCC register
    UART0_CC_R = 0x0;          

    // 6. Enable the UART by setting the UARTEN bit in the UARTCTL register.
    UART0_CTL_R = (1<<0)|(1<<8)|(1<<9); 
    
    // Configure LED pins
    SYSCTL_RCGCGPIO_R |= (1<<5); // enable clock on PortF
    GPIO_PORTF_DIR_R = (1<<1)|(1<<2)|(1<<3);  // make LED pins (PF1, PF2, and PF3) outputs
    GPIO_PORTF_DEN_R = (1<<1)|(1<<2)|(1<<3); // enable digital function on LED pins
    GPIO_PORTF_DATA_R &= ~((1<<1)|(1<<2)|(1<<3)); // turn off leds

    while(1)
    {
        printString("Type 'r','b' or 'g' : ");
        c = readChar();
        printChar(c);
        printString("\n\r");
        switch (c){
        case 'r' : case 'R' : GPIO_PORTF_DATA_R = (1 << 1 ) ;
          break ;
          case 'b' : case 'B' : GPIO_PORTF_DATA_R = (1 << 2 ) ;
          break ;
          case 'g' : case 'G' : GPIO_PORTF_DATA_R = (1 << 3 ) ;
          break ;
          
        }
    
    }
}

char readChar(void)  
{
    char c;
    while((UART0_FR_R & (1<<4)) != 0); 
    c = UART0_DR_R;                  
    return c;                    
}

void printChar(char c)  
{
    while((UART0_FR_R & (1<<5)) != 0);
    UART0_DR_R = c;           
}

void printString(char * string)
{
  while(*string)
  {
    printChar(*(string++));
  }
}


  

