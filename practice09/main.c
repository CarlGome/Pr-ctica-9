////+++++++++++++++++++++++++++++++++++++| LIBRARIES / HEADERS |+++++++++++++++++++++++++++++++++++++
#include <proc/pic18f45k50.h>

#include "device_config.h"

#define _XTAL_FREQ 1000000
#define ONE_SECOND 1000 
#define BUTTON 
#define V1 LATDbits.LATD2
#define A1 LATDbits.LATD1
#define R1 LATDbits.LATD0
#define V2 LATDbits.LATD6
#define A2 LATDbits.LATD5
#define R2 LATDbits.LATD4
#define S1 LATDbits.LATD3
#define S2 LATDbits.LATD7


void portsInit( void );
void delay_1s (void);
void interrupt_int0(void);

void main( void ){
    portsInit();
    RCONbits.IPEN = 1; // Permitir prioridad
    INTCON2bits.INTEDG0 = 1; // Transición positiva
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1; // Permitir interrupción
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    while(1){
            V1 = 0; R1 = 1;A1 = 0;V2 = 1;R2 = 0;A2 = 0;S1 = 0;S2 = 0 //Estado 1 que dura 3s
            delay_1s();
            delay_1s();
            delay_1s();
            V1 = 0; R1 = 1;A1 = 0;V2 = 0;R2 = 0;A2 = 1;S1 = 0;S2 = 0;//Estado 2 que dura 1s
            delay_1s();
            V1 = 1; R1 = 0;A1 = 0;V2 = 0;R2 = 1;A2 = 0;S1 = 0;S2 = 0;//Estado 4 que dura 3s
            delay_1s();
            delay_1s();
            delay_1s();
            V1 = 0; R1 = 0;A1 = 1;V2 = 0;R2 = 1;A2 = 0;S1 = 0;S2 = 0;//Estado 5 que dura 1s
            delay_1s();    
    }
}

void __interrupt (high_priority) interrupt_int0(void){
    __delay_ms(20); // Delay para ekiminar e rebote
    if (PORTBbits.RB0 == 1){ // Revisar si el botón está estable en 1
        V1 = 0; R1 = 1;A1 = 0;V2 = 0;R2 = 1;A2 = 0;S1 = 1;S2 = 1; // Estado 3
        delay_1s();
        delay_1s();}
        INTCONbits.INT0IF = 0; //Apagar bandera
}

void delay_1s(void){ //Utilizamos el timer 0 para un delay de 1 segundo
    // Según nuestros cálculos la cuenta empieza desde 3036 = 0x0BDC
    TMR0H = 0x0B;
    TMR0L = 0xDC;
    INTCONbits.TMR0IF = 0; // Apagamos la bandera del timer
    T0CON = 0b10000101;
    while (INTCONbits.TMR0IF == 0); // Esperamos por el sobre-flujo
    T0CON = 0x00; //Apagamos el timer
}

void portsInit( void ){
    OSCCON = 0b01111110; // Oscilador de 16Mhz
    ANSELD = 0;  // Puerto D digital
    TRISD = 0;   // Puerto D como salidas           
    ANSELB = 0; // Puerto B digital
    TRISBbits.TRISB2 = 1; // Pin RB2 como entrada
    TRISBbits.TRISB0 = 1;  // Pin RB0 como entrada

}