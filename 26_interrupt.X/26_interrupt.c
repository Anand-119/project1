/*
 * File:   26_interrupt.c
 * Author: DELL
 *
 * Created on 5 March, 2024, 3:42 PM
 */
// PIC16F877A Configuration Bit Settings
// 'C' source line config statements
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

 

#include <xc.h>

    void __interrupt () ISR()
    {
    if(INTCONbits.INTF==1)
    {
        PORTCbits.RC0 = ~PORTCbits.RC0;
        INTCONbits.INTF = 0;
    }
    }

void main(void)
{
    INTCONbits.GIE = 1;
    
    INTCONbits.PEIE = 1;
    
    INTCONbits.INTE = 1;
    
    
    OPTION_REGbits.INTEDG = 1;
    TRISC0 = 0;
    
    while(1)
    {
        
    }
    return;
    
    
}
