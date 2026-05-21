#include "functions.h"

// 1. Functia de citire ADC
unsigned int ADC_Read(unsigned char channel) {
    ADCON0 &= 0xC3;            
    ADCON0 |= (channel << 2);  
    __delay_ms(2);             
    GO_nDONE = 1;              
    while (GO_nDONE);
    return (unsigned int)((ADRESH << 8) + ADRESL);
}

// 2. Functia de calcul Temperatura
void calc_temp(){
    rawT = ADC_Read(0);
    temp = (rawT * 500.0) / 1023.0; 
}

// 3. Functia de calcul Lux (7 Trepte)
void calc_lux() {
    rawL = ADC_Read(1);
    vL = (rawL * 5.0) / 1023.0;
    
    if(vL < 0.01) vL = 0.01;
    rLdr = (10000.0 * (5.0 - vL)) / vL;
    
    if (rLdr <= 298.0)       lux = (657900.0 / rLdr) - 707.0;
    else if (rLdr <= 397.0)  lux = (597400.0 / rLdr) - 504.0;
    else if (rLdr <= 567.0)  lux = (529800.0 / rLdr) - 334.0;
    else if (rLdr <= 921.0)  lux = (442500.0 / rLdr) - 180.0;
    else if (rLdr <= 1990.0) lux = (343100.0 / rLdr) - 72.0;
    else if (rLdr <= 3242.0) lux = (257600.0 / rLdr) - 29.0;
    else                     lux = (192100.0 / rLdr) - 9.0;
    
    if (lux < 0.0) lux = 0.0;
}

// 4. Functia de calcul Umiditate
// 4. Functia de calcul Umiditate (Calibrata pentru senzor HIH-5050)
void calc_humid(){
    rawH = ADC_Read(2);
    
    // 1. Aflam tensiunea reala citita pe pinul AN2 (intre 0V si 5V)
    float vH = (rawH * 5.0) / 1023.0; 
    
    // 2. Aplicam formula inversa din datasheet-ul senzorului (Vsupply = 5.0)
    humid = ((vH / 5.0) - 0.1515) / 0.00636;
    
    // 3. Filtre de siguranta pentru ca afisajul sa nu iasa din limitele logice
    if (humid < 0.0) humid = 0.0;
    if (humid > 100.0) humid = 100.0;
}

// 5. Formatare Rand 1 (Temperatura si Lux)
void format_row1() {
    for(int i=0; i<16; i++) lcd_msg[i] = ' ';
    lcd_msg[16] = '\0';

    lcd_msg[0] = 'T'; lcd_msg[1] = ':';
    int idx = 2;
    if (t_w >= 100) lcd_msg[idx++] = (char)((t_w / 100) % 10 + '0');
    if (t_w >= 10)  lcd_msg[idx++] = (char)((t_w / 10) % 10 + '0');
    lcd_msg[idx++] = (char)((t_w % 10) + '0');
    lcd_msg[idx++] = 'C';
    
    lcd_msg[idx++] = ' ';
    lcd_msg[idx++] = 'L'; lcd_msg[idx++] = ':';
    if (l_w >= 10000) lcd_msg[idx++] = (char)((l_w / 10000) % 10 + '0');
    if (l_w >= 1000)  lcd_msg[idx++] = (char)((l_w / 1000) % 10 + '0');
    if (l_w >= 100)   lcd_msg[idx++] = (char)((l_w / 100) % 10 + '0');
    if (l_w >= 10)    lcd_msg[idx++] = (char)((l_w / 10) % 10 + '0');
    lcd_msg[idx++] = (char)(l_w % 10 + '0');
}

// 6. Formatare Rand 2 (Umiditate si Alerta Lux)
void format_row2() {
    // Alarma vizuala de lumina mare
    if (l_w > 2000) {
        char alert_msg[] = "LUMINA PREA MARE";
        for(int i = 0; i < 16; i++) lcd_msg[i] = alert_msg[i];
        lcd_msg[16] = '\0';
    } 
    else {
        // Afisarea normala
        for(int i=0; i<16; i++) lcd_msg[i] = ' ';
        lcd_msg[16] = '\0';

        lcd_msg[0] = 'U'; lcd_msg[1] = ':';
        int idx = 2;
        if (h_w >= 100) {
            lcd_msg[idx++] = '1'; lcd_msg[idx++] = '0'; lcd_msg[idx++] = '0';
        } else {
            if (h_w >= 10) lcd_msg[idx++] = (char)((h_w / 10) % 10 + '0');
            lcd_msg[idx++] = (char)((h_w % 10) + '0');
        }
        lcd_msg[idx++] = '%';
        
        // Semnatura
        lcd_msg[7] = 'V'; lcd_msg[8] = 'L'; lcd_msg[9] = 'A'; lcd_msg[10] = 'D'; 
        lcd_msg[11] = ' '; lcd_msg[12] = 'H'; lcd_msg[13] = '.';
    }
}