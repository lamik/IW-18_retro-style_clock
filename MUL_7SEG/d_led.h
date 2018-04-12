#ifndef _d_led_h		// dyrektywy zabezpieczaj�ce przed wielokrotnym do��czaniem
#define _d_led_h		// tego samego pliku nag��wkowego je�li b�dzie do��czany
				// w wielu r�nych plikach programu
#include <avr/io.h>			// do��czenie g��wnego systemowego  pliku nag��wkowego
#include <avr/interrupt.h>	// do��czenie pl. nag��wkowego potrzebnego do obs�. przerwa�
#include <avr/pgmspace.h>	// do��czenie pl. nag��wkowego potrzebnego do odczytu
							// danych zawartych w pami�ci programu FLASH

// definicje port�w i pin�w wy�wietlacza u�ywanych dla u�atwienia w programie jako sta�e preprocesora
#define 	LED_DATA 	PORTD	// port z pod��czonymi segmentami
#define		LED_DATA_DIR DDRD	// rejestr kierunku portu katod wy�wietlaczy
#define 	ANODY_PORT 	PORTA	// port z pod��czonymi anodami- 4 bity najm�odsze
#define 	ANODY_DIR 	DDRA	// rejestr kierunku portu anod wy�wietlaczy

#define 	CA1 	(1<<PA0)
#define 	CA2 	(1<<PA7)
#define 	CA3 	(1<<PA1)
#define 	CA4 	(1<<PA6)
#define 	CA5 	(1<<PA2)
#define 	CA6 	(1<<PA5)
#define 	CA7 	(1<<PA4)
#define 	CA8 	(1<<PA3)

// definicje bit�w dla poszczeg�lnych segment�w LED
#define SEG_A (1<<PD0)
#define SEG_B (1<<PD2)
#define SEG_C (1<<PD5)
#define SEG_D (1<<PD6)
#define SEG_E (1<<PD4)
#define SEG_F (1<<PD1)
#define SEG_G (1<<PD3)
#define KROPKA (1<<PD7)



// same DEKLARACJE zmiennych globalnych na potrzeby wykorzystania ich w innych plikach projektu
// przydomek externpowoduje, i� te zmienne b�d� dost�pne we wszystkich modu�ach, kt�re do��cz� plik
// d_led.h za pomoc� dyrektywy #include
extern volatile uint8_t cy1;
extern volatile uint8_t cy2;
extern volatile uint8_t cy3;
extern volatile uint8_t cy4;
extern volatile uint8_t cy5;
extern volatile uint8_t cy6;
extern volatile uint8_t cy7;
extern volatile uint8_t cy8;

void d_led_init(void);
uint8_t z_kropka(uint8_t a);

#endif	// koniec _d_led_h

