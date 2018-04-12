/*                  e-gadget.header
 * rtc.h
 *
 *  Created on: 29-07-2013
 *    Modyfied: 2015-05-21 09:56:10
 *      Author: Mateusz Salamon
 *
 *          MCU: ATmega32
 *        F_CPU: 16 000 000 Hz
 *
 */

#include <stdlib.h>
#include "avr/io.h"
#include "../I2C_TWI/i2c_twi.h"
#include "../common.h"

#ifndef RTC_H_
#define RTC_H_
/*========USTAWIENIA DLA LCD============*/
#define DATETIME_AS_STRING 0 // 1 - konwersja czasu i daty na string, 0 - bez konwersji

/*======================================*/

/*===========ADRES RTC==================*/
#define PCF8583_ADDRESS 0xA0 // A1 -> GND
//#define PCF8583_ADDRESS 0xA2 // A1 -> VCC
/*======================================*/

#define DATE_SEPARATOR '-'

typedef enum { pon, wto, sro, czw, pia, sob, nie } TDAYS;

typedef struct
{
	uint8_t hh;
	uint8_t mm;
	uint8_t ss;
	uint16_t YY; //pelny rok
	uint8_t MM;
	uint8_t DD;
	TDAYS weekday; //dzien tygodnia
	uint8_t dst; //czas letni/zimowy
#if DATETIME_AS_STRING == 1
	char time[9];
	char date[11];
#endif
	uint8_t pcf_buf[5];
} TDATETIME;

extern TDATETIME datetime;



uint8_t dec2bcd(uint8_t dec); //zamiana dziesietnych na bcd
uint8_t bcd2dec(uint8_t bcd); //zamiana bcd na dziesietne

void register_pcf8583_callback( void (*callback)(TDATETIME *dt) );
void rtc_init();
void RTC_EVENT();

void rtc_odczytaj_czas_date(TDATETIME *dt);
void rtc_ustaw_czas(TDATETIME *dt, uint8_t hh, uint8_t mm, uint8_t ss);
void rtc_ustaw_date(TDATETIME *dt, uint16_t YY, uint8_t MM, uint8_t DD);
void rtc_ustaw_czas_date(TDATETIME *dt, uint8_t hh, uint8_t mm, uint8_t ss, uint16_t YY, uint8_t MM, uint8_t DD);

void rtc_dodaj_godzine(TDATETIME *dt);
void rtc_dodaj_minute(TDATETIME *dt);
void rtc_dodaj_rok(TDATETIME *dt);
void rtc_dodaj_miesiac(TDATETIME *dt);
void rtc_dodaj_dzien(TDATETIME *dt);

#endif /* RTC_H_ */

