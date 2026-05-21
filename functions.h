#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Frecventa definita aici pentru a fi vizibila peste tot
#define _XTAL_FREQ 8000000 
#include <xc.h> 

// Impartim variabilele globale cu restul proiectului
extern unsigned int rawT, rawH, rawL;
extern float temp, lux, humid, vL, rLdr;
extern char lcd_msg[17];
extern unsigned int t_w, h_w, l_w; 

// Prototipuri
unsigned int ADC_Read(unsigned char channel);
void calc_lux(void);
void calc_temp(void);
void calc_humid(void);
void format_row1(void);
void format_row2(void);

#endif