#ifndef _d_led_h		// dyrektywy zabezpieczaj¹ce przed wielokrotnym do³¹czaniem
#define _d_led_h		// tego samego pliku nag³ówkowego jeœli bêdzie do³¹czany
				// w wielu ró¿nych plikach programu
#include <avr/io.h>			// do³¹czenie g³ównego systemowego  pliku nag³ówkowego
#include <avr/interrupt.h>	// do³¹czenie pl. nag³ówkowego potrzebnego do obs³. przerwañ
#include <avr/pgmspace.h>	// do³¹czenie pl. nag³ówkowego potrzebnego do odczytu
							// danych zawartych w pamiêci programu FLASH

// definicje portów i pinów wyœwietlacza u¿ywanych dla u³atwienia w programie jako sta³e preprocesora
#define 	LED_DATA 	PORTD	// port z pod³¹czonymi segmentami
#define		LED_DATA_DIR DDRD	// rejestr kierunku portu katod wyœwietlaczy
#define 	ANODY_PORT 	PORTA	// port z pod³¹czonymi anodami- 4 bity najm³odsze
#define 	ANODY_DIR 	DDRA	// rejestr kierunku portu anod wyœwietlaczy

#define 	CA1 	(1<<PA0)
#define 	CA2 	(1<<PA7)
#define 	CA3 	(1<<PA1)
#define 	CA4 	(1<<PA6)
#define 	CA5 	(1<<PA2)
#define 	CA6 	(1<<PA5)
#define 	CA7 	(1<<PA4)
#define 	CA8 	(1<<PA3)

// definicje bitów dla poszczególnych segmentów LED
#define SEG_A (1<<PD0)
#define SEG_B (1<<PD2)
#define SEG_C (1<<PD5)
#define SEG_D (1<<PD6)
#define SEG_E (1<<PD4)
#define SEG_F (1<<PD1)
#define SEG_G (1<<PD3)
#define KROPKA (1<<PD7)



// same DEKLARACJE zmiennych globalnych na potrzeby wykorzystania ich w innych plikach projektu
// przydomek externpowoduje, i¿ te zmienne bêd¹ dostêpne we wszystkich modu³ach, które do³¹cz¹ plik
// d_led.h za pomoc¹ dyrektywy #include
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

