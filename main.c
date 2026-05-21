#pragma config FOSC = INTRC_NOCLKOUT, WDTE = OFF, PWRTE = OFF, MCLRE = OFF, CP = OFF, LVP = OFF

#include <xc.h>
#include "LCD.h"
#include "functions.h"

#define LED_ROSU PORTEbits.RE0
#define LED_BLUE PORTEbits.RE1

#define TEMP_MIN 20.0
#define TEMP_MAX 25.0
#define HUMID_MIN 40.0  
#define HUMID_MAX 60.0  

// Alocarea memoriei globale 
unsigned int rawT, rawH, rawL;
float temp, lux, humid, vL, rLdr;
char lcd_msg[17];
unsigned int t_w, h_w, l_w; 

void main(void) {
    OSCCON = 0x71;      
    
    ANSEL = 0b00000111; // AN0, AN1, AN2 active ca intrari de senzor
    ANSELH = 0x00;      // Restul pinilor sunt digitali
    
    TRISA = 0b00000111; // Intrarile pentru senzori (PORTA)
    TRISD = 0x00;       // Iesirile pentru LCD (PORTD)
    TRISE = 0x00;       // Iesirile pentru LED-uri (PORTE)
    
    PORTD = 0x00; 
    PORTE = 0x00;
    
    ADCON0 = 0x81;      
    ADCON1 = 0x80;      

    lcd_init();

    while(1) {
        // Citire din functii
        calc_temp();
        calc_lux();
        calc_humid();

        // Control LED Rosu (Temperatura)
        if (temp < TEMP_MIN || temp > TEMP_MAX) LED_ROSU = 1;
        else LED_ROSU = 0;

        // Control LED Albastru (Umiditate)
        if (humid < HUMID_MIN || humid > HUMID_MAX) LED_BLUE = 1;
        else LED_BLUE = 0;

        // Conversie explozita in intregi (pentru printare rapida)
        t_w = (unsigned int)temp;                    
        h_w = (unsigned int)humid;
        l_w = (unsigned int)lux;

        // Randul 1 (Temperatura si Lux)
        format_row1();
        lcd_goto(0x00);
        lcd_puts(lcd_msg);

        // Randul 2 (Umiditate / Nume sau Alerta)
        format_row2();
        lcd_goto(0x40);
        lcd_puts(lcd_msg);

        __delay_ms(500); // Rata de refresh
    }
}