/*                  e-gadget.header
 * rtc.c
 *
 *  Created on: 29-07-2013
 *    Modyfied: 2015-05-21 09:56:10
 *      Author: Mateusz Salamon
 *
 *          MCU: ATmega32
 *        F_CPU: 16 000 000 Hz
 *
 */


#include "pcf8583.h"

TDATETIME datetime;

volatile uint8_t sek, min, h;
volatile uint8_t rok=12, dzien, miesiac;

//zamiana liczb
uint8_t dec2bcd(uint8_t dec)
{
	return ((dec/10<<4|(dec%10)));
}
uint8_t bcd2dec(uint8_t bcd)
{
	return ((((bcd)>>4&0x0F)*10)+((bcd)&0xF));
}
//Wskazniki do funkcji callback RTC
static void (*pcf8583_callback)(TDATETIME *dt);

//Rejestracja callbacka RTC
void register_pcf8583_callback( void (*callback)(TDATETIME *dt) )
{
	pcf8583_callback = callback;
}

void rtc_init()
{
	//Wyzwalanie niskim stanem jest fabrycznie
	PORTB |= (1<<PB2); //podciagniecie do vcc
}

void RTC_EVENT()
{
	if(GIFR & (1<<INTF2)) //sprawdzenie sprzetowej flagi
	{
		rtc_odczytaj_czas_date( &datetime );

		if(pcf8583_callback) pcf8583_callback( &datetime);

		GIFR |= (1<<INTF2); //Zerowanie flagi wpisaniem jedynki
	}
}

void rtc_odczytaj_czas_date(TDATETIME *dt)
{
	TWI_read_buf(PCF8583_ADDRESS, 2, 5, dt->pcf_buf); //wczytanie danych do bufora
	TWI_read_buf(PCF8583_ADDRESS, 16, 2, (uint8_t*)&dt->YY); //wczytanie pelnego roku

	int8_t i;
	uint8_t * wsk = dt->pcf_buf;
#if DATETIME_AS_STRING == 1
	char *znak = dt->time;
#endif
	for(i=2; i>-1; i--) //od drugiego elementu bufora(godziny)
	{
#if DATETIME_AS_STRING == 1
	*(znak++) = ( (*(wsk+i) & (2==i? 0x3f:0x7f) ) >> 4) + '0'; // dziesiatki wyciagniete z BCD + maskowanie AM/PM, trybu 24/12h
	*(znak++) = ( *(wsk+i) & 0x0f ) + '0'; //jednostki wyciagniete z BCD
	*(znak++) = i? ':':0; //separator lub '0' na koncu stringa
#endif
	*( (uint8_t*)dt+2-i) = bcd2dec( *(wsk+i)); //Zamiana struktury na wskaznik, rozpoczecie od pierwszego elementu czyli godzin i zamiana bcd na dec
	}

	dt->DD = bcd2dec( *(wsk+3) & 0x3f ); //wpis dni z maskowaniem roku
	uint8_t yr = wsk[3] >> 6; //zmienna pomocnicza na trzymanie dwoch ostatnich bitow roku

	if(dt->YY >2100) dt->YY = 2015; //niepotrzebny rok ponizej

	//warunek adziala tylko na sylwestra
	if( (dt->YY & 3) != yr) // jesli dwa bity pelnego roku rownaja sie zmiennej yr
	{
		while( (dt->YY & 3) != yr ) dt->YY++; //jesli inny to zwiekszaj dopoki sie nie wyrowna
		TWI_write_buf(PCF8583_ADDRESS, 0x10, 2, (uint8_t*)&dt->YY); //wpisz nowa wartosc pelnego roku do RAM PCF8583
	}

	dt->MM = bcd2dec( *(wsk+4) & 0x1f ); //wpis dni z maskowaniem dnia tygodnia
	dt->weekday = wsk[4] >> 5; //dni tygodnia

#if DATETIME_AS_STRING == 1
	//format: DD-MM-RRRR
	znak = dt->date;

	//zamiana dni
	*(znak++) = ( (wsk[3] & 0x3f ) >> 4 ) + '0'; //
	*(znak++) = ( (wsk[3] & 0x0f ) ) + '0'; //

	*(znak++) = DATE_SEPARATOR;
	//zamiana miesiaca na ascii
	*(znak++) = ( ( wsk[4] & 0x1f ) >> 4 ) +'0'; //
	*(znak++) = ( ( wsk[4] & 0x0f ) ) + '0'; //

	*(znak++) = DATE_SEPARATOR;

	itoa(dt->YY, znak, 10); //zamiana roku na ascii
#endif
}

//zapis godziny do zegara
void rtc_ustaw_czas(TDATETIME *dt, uint8_t hh, uint8_t mm, uint8_t ss)
{
	dt->pcf_buf[0] = dec2bcd(ss);
	dt->pcf_buf[1] = dec2bcd(mm);
	dt->pcf_buf[2] = dec2bcd(hh);
	TWI_write_buf(PCF8583_ADDRESS, 0x02, 3, dt->pcf_buf);
}

void rtc_ustaw_date(TDATETIME *dt, uint16_t YY, uint8_t MM, uint8_t DD)
{
	dt->pcf_buf[3] = dec2bcd(DD) | ((YY & 0x03) << 6 ); //wrzucanie dnia oraz dwoch bitow roku
	dt->pcf_buf[4] = dec2bcd(MM); // | (dzien tygodnia);
	TWI_write_buf(PCF8583_ADDRESS, 0x05, 2, &dt->pcf_buf[3]);
	TWI_write_buf(PCF8583_ADDRESS, 0x10, 2, (uint8_t*)&YY);
}

void rtc_ustaw_czas_date(TDATETIME *dt, uint8_t hh, uint8_t mm, uint8_t ss, uint16_t YY, uint8_t MM, uint8_t DD)
{
	rtc_ustaw_czas(dt, hh, mm, ss);
	rtc_ustaw_date(dt, YY, MM, DD);
}

void rtc_dodaj_godzine(TDATETIME *dt)
{
	if(dt->hh<23)
		dt->hh++;
	else dt->hh = 0;
	dt->ss = 0;

	rtc_ustaw_czas(dt, dt->hh, dt->mm, dt->ss);

	Timer2=500; //odswiezenie oczekiwania na akonczenie ustawien
}

void rtc_dodaj_minute(TDATETIME *dt)
{

	if(dt->mm<59)
		dt->mm++;
	else dt->mm = 0;
	dt->ss = 0;

	rtc_ustaw_czas(dt, dt->hh, dt->mm, dt->ss);

	Timer2=500;
}

void rtc_dodaj_rok(TDATETIME *dt)
{
	if(dt->YY<2015) dt->YY = 2015;
	if(dt->YY<2100) dt->YY++;
	else dt->YY = 2015;

	rtc_ustaw_date(dt, dt->YY, dt->MM, dt->DD);

	Timer2=500;
}

void rtc_dodaj_miesiac(TDATETIME *dt)
{

	if(dt->MM<12)
	dt->MM++;
	else dt->MM = 1;

	rtc_ustaw_date(dt, dt->YY, dt->MM, dt->DD);

	Timer2=500;
}

void rtc_dodaj_dzien(TDATETIME *dt)
{
	if((dt->MM==1||dt->MM==3||dt->MM==5||dt->MM==7||dt->MM==8||dt->MM==10||dt->MM==12)&& dt->DD<31)
		dt->DD++;
	else if((dt->MM==4||dt->MM==6||dt->MM==9||dt->MM==11) && dt->DD<30)
		dt->DD++;
	else if(dt->MM==2 && (dt->YY%4)==0 && dt->DD<29)
		dt->DD++;
	else if(dt->MM==2 &&(dt->YY%4==1||dt->YY%4==2||dt->YY%4==3) && dt->DD<28)
		dt->DD++;
	else dt->DD = 1;

	rtc_ustaw_date(dt, dt->YY, dt->MM, dt->DD);

	Timer2=500;
}

