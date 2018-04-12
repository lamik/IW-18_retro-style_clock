/*                  e-gadget.header
 * main.c
 *
 *  Created on: 18-01-2014
 *    Modyfied: 2015-05-21 09:56:10
 *      Author: Mateusz Salamon
 *
 *          MCU: ATmega32
 *        F_CPU: 16 000 000 Hz
 *
 */

#include "MUL_7SEG/d_led.h"
#include "PCF8583/pcf8583.h"
#include "MENU/menu.h"
#include "DS18X20/ds18x20.h"
#include "DHT/dht.h"
#include "MPL115A2/MPL115A2.h"

int main(void)
{
	//przyciski
	DDRB &=~(KEY1|KEY2);
	PORTB |= KEY1|KEY2;
	//

	//Timer 2 do timerow programowych
	TCCR2 |= (1<<WGM21)|(1<<CS22)|(1<<CS21)|(1<<CS20); // CTC, preskaler 1024
	OCR2 = 155; // dla przerwania co 10ms
	TIMSK |= (1<<OCIE2); //zezwolenie na przerwanie
	//

	i2cSetBitrate(100);
	d_led_init();
	rtc_init();
	sei();

	czujniki_cnt = search_sensors();
	DS18X20_start_meas(DS18X20_POWER_EXTERN, NULL);


	MPL115A2_start_conversion();
	MPL115A2_read_coeffs();


	//pressure = 1025;

	//rtc_ustaw_czas_date(&datetime, 23, 59, 57, 2013, 12, 31);

	register_pcf8583_callback(wyswietlaj); //odswiezanie wyswietlaczy wraz z odczytem RTC

	while(1)
		{

		RTC_EVENT(); //co sekunde odczyt czasu z rtc

		SuperDebounce(&PINB,KEY1,2,50,ustawienia,0, &datetime); //wejscie w ustawienia zegara

		if(!Timer3) // co sekunde
		{
			//odebranie temperatury i ponowny pomiar
			DS18X20_read_meas(gSensorIDs[0],&subzero,&cel,&cel_fract_bits);
			DS18X20_start_meas(DS18X20_POWER_EXTERN, NULL);

			Timer3 = 100; //wyzerowanie flagi
		}

		if(!Timer2) //co 7s
		{
//			menu=5;
			if(menu<ILOSC_MENU) menu++;
			else menu=1;

			dht_gethumidity(&wilgotnosc);
			MPL115A2_get_pressure_and_temperature();
			MPL115A2_start_conversion();

			Timer2 = 700;
		}

		}
}

ISR(TIMER2_COMP_vect)
{
	//przerwanie co 10ms
	uint16_t n;
//	Timery do obslugi SCR
	n = Timer1;		/* 100Hz Timer2 */
	if (n) Timer1 = --n;
	n = Timer2;		/* 100Hz Timer3 */
	if (n) Timer2 = --n;
	n = Timer3;		/* 100Hz Timer3 */
	if (n) Timer3 = --n;

	n = TimerDebounce;		/* Timer przyciskow */
	if (n) TimerDebounce = --n;
}


