#ifndef LCD_H
#define LCD_H

#include <xc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

// Definitii Pini - Legaturile fizice
#define RS PORTDbits.RD0
#define EN PORTDbits.RD1
#define D4 PORTDbits.RD2
#define D5 PORTDbits.RD3
#define D6 PORTDbits.RD4
#define D7 PORTDbits.RD5

// Prototipuri functii LCD
void lcd_init(void);
void lcd_cmd(unsigned char cmd);
void lcd_data(char data);
void lcd_puts(char *str);
void lcd_goto(unsigned char pos);

#endif