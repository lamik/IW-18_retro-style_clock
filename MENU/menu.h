/*                  e-gadget.header
 * menu.h
 *
 *  Created on: 19-02-2014
 *    Modyfied: 2015-05-21 09:56:10
 *      Author: Mateusz Salamon
 *
 *          MCU: ATmega32
 *        F_CPU: 16 000 000 Hz
 *
 */

#ifndef MENU_H_
#define MENU_H_

#include <avr/io.h>
#include <stddef.h>

#include "../PCF8583/pcf8583.h"
#include "../common.h"
#include "../MUL_7SEG/d_led.h"
#include "../DS18X20/ds18x20.h"
#include "../DHT/dht.h"
#include "../MPL115A2/MPL115A2.h"

#define ILOSC_MENU 5

#define KRESKA 16
#define PUSTY 17

#define  KEY1 (1<<PB0)
#define  KEY2 (1<<PB1)
#define REP_WAIT 100	// Oczekiwanie na fukncje repeat X*10ms
#define REP_TIME 35	// Czas pomiedzy cyklami repeat X*10ms

uint8_t pozycja;
extern uint8_t menu;

void register_rep_proc_callback( void (*callback)(TDATETIME *dt) );
void register_push_proc_callback( void (*callback)(TDATETIME *dt) );

void SuperDebounce(volatile uint8_t *KPIN,
		uint8_t key_mask, uint16_t rep_time, uint16_t rep_wait,
		void (*push_proc_callback)(TDATETIME *dt), void (*rep_proc_callback)(TDATETIME *dt),// );
		TDATETIME *dt);

void nastepne();
void ustawienia(TDATETIME *dt);
void czas(TDATETIME *dt);
void data(TDATETIME *dt);
void temperatura(uint8_t cel, uint8_t cel_fract_bits);
void huminidy( uint8_t wilgotnosc );
void cisnienie( uint16_t pressure);
void wyswietlaj(TDATETIME *dt);

#endif /* MENU_H_ */

