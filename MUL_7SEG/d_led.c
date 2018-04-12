
#include "d_led.h"			// do��czenie naszego pliku nag��wkowego
							// w nim znajduj� si� potrzebne tu m.in. definicje preprocesora

// definicje zmiennych globalnych przechowuj�cych cyfry do wy�wietlania
// volatile � poniewa� b�d� wykorzystywane do odczytu i zapisu zar�wno w przerwaniu i programie
// g��wnym. Trzeba wi�c wy��czy� optymalizacj� dost�pu do nich. (zmienne ulotne)
volatile uint8_t cy1;
volatile uint8_t cy2;
volatile uint8_t cy3;
volatile uint8_t cy4;
volatile uint8_t cy5;
volatile uint8_t cy6;
volatile uint8_t cy7;
volatile uint8_t cy8;

// definicja tablicy zawieraj�cej definicje bitowe cyfr LED
uint8_t const cyfry[30] PROGMEM = {
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F),			// 0
		~(SEG_B|SEG_C),									// 1
		~(SEG_A|SEG_B|SEG_D|SEG_E|SEG_G),				// 2
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_G),				// 3
		~(SEG_B|SEG_C|SEG_F|SEG_G),						// 4
		~(SEG_A|SEG_C|SEG_D|SEG_F|SEG_G),				// 5
		~(SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G),			// 6
		~(SEG_A|SEG_B|SEG_C|SEG_F),						// 7
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G),	// 8
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G),			// 9
		~(SEG_A|SEG_B|SEG_G|SEG_F),						//stopien  10
		~(SEG_A|SEG_F|SEG_E|SEG_D),						//C			11
		~(SEG_F|SEG_E|SEG_G|SEG_C),						//h			12
		~(SEG_A|SEG_B|SEG_G|SEG_F|SEG_E),				//P			13
		~(SEG_A|SEG_B|SEG_C|SEG_E|SEG_F|SEG_G),			//A			14
		~(SEG_E|SEG_G),									//r			15
		~(SEG_G),										//-			16
		0xFF,											// NIC (puste miejsce)	17
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|KROPKA),			// 18
		~(SEG_B|SEG_C|KROPKA),									// 19
		~(SEG_A|SEG_B|SEG_D|SEG_E|SEG_G|KROPKA),				// 20
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_G|KROPKA),				// 21
		~(SEG_B|SEG_C|SEG_F|SEG_G|KROPKA),						// 22
		~(SEG_A|SEG_C|SEG_D|SEG_F|SEG_G|KROPKA),				// 23
		~(SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G|KROPKA),			// 24
		~(SEG_A|SEG_B|SEG_C|SEG_F|KROPKA),						// 25
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G|KROPKA),	// 26
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G|KROPKA)			// 27

};

// ****** definicja funkcji inicjalizuj�cej prac� z wy�wietlaczem multipleksowanym
void d_led_init(void)
{
	LED_DATA_DIR = 0xFF;   					// wszystkie piny portu D jako WYJ�CIA(katody)
	LED_DATA = 0xFF;						// wygaszenie wszystkich katod � stan wysoki
	ANODY_DIR |= CA1 | CA2 | CA3 | CA4 | CA5 | CA6 | CA7 | CA8;		// 8 pinow portu A jako WYJ�CIA (anody wy�wietlaczy)
	ANODY_PORT |= CA1 | CA2 | CA3 | CA4 | CA5 | CA6 | CA7 | CA8;	// wygaszenie wszystkich wy�wietlaczy - anody

	// ustawienie TIMER0
	TCCR0 |= (1<<WGM01);				// tryb CTC
	TCCR0 |= (1<<CS02)|(1<<CS00);		// preskaler = 1024
	OCR0 = 30;							// dodatkowy podzia� przez 38 (rej. przepe�nienia)
	TIMSK |= (1<<OCIE0);				// zezwolenie na przerwanie CompareMatch
}

uint8_t z_kropka(uint8_t a)
{
	return a+18;
}

// ================= PROCEDURA OBS�UGI PRZERWANIA � COMPARE MATCH
ISR(TIMER0_COMP_vect)
{
	static uint8_t licznik=1;		// zmienna do prze��czania kolejno anod wyrwietlacza

	ANODY_PORT = (ANODY_PORT & 0xFF);	// wygaszenie wszystkich wy�wietlaczy

	if(licznik==CA1) 		LED_DATA = pgm_read_byte( &cyfry[cy8] );	// gdy zapalony wy�w.1 podaj stan zmiennej c1
	else if(licznik==CA2) LED_DATA = pgm_read_byte( &cyfry[cy7] );	// gdy zapalony wy�w.2 podaj stan zmiennej c2
	else if(licznik==CA3) LED_DATA = pgm_read_byte( &cyfry[cy6] );	// gdy zapalony wy�w.3 podaj stan zmiennej c3
	else if(licznik==CA4) LED_DATA = pgm_read_byte( &cyfry[cy5] );	// gdy zapalony wy�w.4 podaj stan zmiennej c4
	else if(licznik==CA5) LED_DATA = pgm_read_byte( &cyfry[cy4] );	// gdy zapalony wy�w.2 podaj stan zmiennej c5
	else if(licznik==CA6) LED_DATA = pgm_read_byte( &cyfry[cy3] );	// gdy zapalony wy�w.3 podaj stan zmiennej c6
	else if(licznik==CA7) LED_DATA = pgm_read_byte( &cyfry[cy2] );	// gdy zapalony wy�w.4 podaj stan zmiennej c7
	else if(licznik==CA8) LED_DATA = pgm_read_byte( &cyfry[cy1] );	// gdy zapalony wy�w.4 podaj stan zmiennej c8

	ANODY_PORT = ~licznik;			// cykliczne prze��czanie kolejnej anody w ka�dym przerwaniu

	// operacje cyklicznego przesuwania bitu zapalaj�cego anody w zmiennej licznik
	licznik <<= 1;					// przesuni�cie zawarto�ci bit�w licznika o 1 w lewo
	if(licznik==0) licznik = 1;		// je�li licznik wi�kszy ni� 8 to ustaw na 1
}

