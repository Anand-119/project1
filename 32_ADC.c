/*
 * File:   32_ADC.c
 * Author: DELL
 *
 * Created on 13 March, 2024, 4:17 PM
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
#define _XTAL_FREQ 20000000

#define EN PORTDbits.RD2
#define RW PORTDbits.RD1
#define RS PORTDbits.RD0

unsigned int a,b,c,d,e,f;
unsigned int temp,adc;
float temperature, adc1;

/*void delay(unsigned int i)        //if you use this function you need to then you need to replace __delay_ms(500) with delay(500)
{
    while(i--);            
}*/
void lcd_command(unsigned char a)       //this function is used for instruction
{
    PORTC = a;
    EN = 1;
    RS = 0;
    RW = 0;
    __delay_ms(500);
    EN = 0;
}
void chr(unsigned char b)               //this function is used for data
{
    PORTC = b;
    EN = 1;
    RS = 1;
    RW = 0;
    __delay_ms(500);
    EN = 0;
}
void str(const char *d, char n)         //here if you remove const also it will work
{
    char j;
    for(j=0; j<n; j++)
    {
        chr(d[j]);
        __delay_ms(10);                //after printing each character delay is provided for the next character
    }
}
void lcd_initialise()                   //this function is used to initialise the LCD
{
    lcd_command(0x38);
    lcd_command(0x06);
    lcd_command(0x0c);
    lcd_command(0x01);
    
}
void __interrupt() adc_conv()       //interrupt is called when AD conversion is started 
{
    if(PIR1bits.ADIF==1)            //checking analog to digital converter interrupt flag is 1
    {
        adc = (ADRESH<<8);          //since we cannot store 10 bits in one register we are shifting 8 times & storing 2bits in ADRESH register
        adc = (adc+ADRESL);         //another 8bits in ADRESL register are added to ADRESH register which is stored in ADC
        PIR1bits.ADIF = 0;          //ADIF flag is reset
    }
}

void main(void)
{
    INTCONbits.GIE = 1;             //for any interrupt GIE and PEIE should be enabled
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;              //enabling analog to digital conversion
    
    TRISD = 0x00;                   //configuring TRISD register as output which is connected to RS, RW and RE
    TRISC = 0x00;                   //configuring TRISC register as output which is connected to data pins of LCD
    
    PORTC = PORTD = 0x00;
    lcd_initialise();
    lcd_command(0x80);              //0x80 represents 1st row and 1st character
    str("SENSOR:", 7);
    
    ADCON0 = 0x41;
    ADCON1 = 0x8E;
    
    while(1)
    {
        ADCON0 = ADCON0 | (0x04);       //when we perform OR operation with ADCON0 register it means that we are starting AD conversion
        adc1 = adc/2.046;               //by knowing the step size ie, 4.8875 mv and step rise in voltage ie, 10 mv of LM35 this factor is calculated
        temperature = adc1*100;         //this expression is used to convert the decimal value to integer value
        lcd_command(0x89);              //this is used to print the character in 1st row and 9th position
        a = (int)temperature/10;        //implicit conversion is done
        b = (int)temperature%10;        //implicit conversion is done
        c = a%10;                       //each digit is extracted
        d = a/10;
        e = d%10;
        f = d/10;
        chr(f+0x30);                    //first one character is passed and printed
        chr(e+0x30);                    //2nd character is passed and printed
        chr('.');                       //. is passed and printed
        chr(c+0x30);            //0x30 is used to convert the value c in to character, here c+48 is done. decimal value of c 0x30 is 48
        chr(b+0x30);            
        
       
    }
    
    
}