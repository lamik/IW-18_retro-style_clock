/*                  e-gadget.header
 * menu.c
 *
 *  Created on: 19-02-2014
 *    Modyfied: 2015-05-21 09:56:10
 *      Author: Mateusz Salamon
 *
 *          MCU: ATmega32
 *        F_CPU: 16 000 000 Hz
 *
 */

#include "menu.h"

//Wskazniki do funkcji callbackow dla przycisniecia klawisza
static void (*push_proc_callback)(TDATETIME *dt);
static void (*rep_proc_callback)(TDATETIME *dt);

uint8_t menu = 1;


//Rejestracja callbackow przyciskow
void register_push_proc_callback( void (*callback)(TDATETIME *dt) )
{
	push_proc_callback = callback;
}

void register_rep_proc_callback( void (*callback)(TDATETIME *dt) )
{
	rep_proc_callback = callback;
}

//Obsluga przyciskow
void SuperDebounce(volatile uint8_t *KPIN,
		uint8_t key_mask, uint16_t rep_time, uint16_t rep_wait,
		void (*push_proc_callback)(TDATETIME *dt), void (*rep_proc_callback)(TDATETIME *dt),// ) {
		TDATETIME *dt){

	enum KS {idle, debounce, go_rep, wait_rep, rep};

	static enum KS key_state;
	static uint8_t last_key;
	uint8_t key_press;

	// zabezpieczenie przed wykonywaniem tej samej funkcji dla
	// dwóch klawiszy wciskanych jednoczeœnie (zawsze bêdzie
	// wykonywana odpowiednia akcja dla tego, który zosta³
	// wciœniêty jako pierwszy
	if( last_key && last_key != key_mask ) return;

	key_press = !(*KPIN & key_mask);

	if( key_press && !key_state ) {
		key_state = debounce;
		TimerDebounce = 5;
	} else
	if( key_state  ) {
		if( key_press && debounce==key_state && !TimerDebounce ) {
			key_state = go_rep;
			TimerDebounce=1;
			last_key = key_mask;
		} else
		if( !key_press && key_state>debounce && key_state<rep ) {
			if(push_proc_callback) push_proc_callback(dt);						/* KEY_UP */
			key_state=idle;
			last_key = 0;
		} else
		if( key_press && go_rep==key_state && !TimerDebounce ) {
			if(!rep_time) rep_time=20;
			if(!rep_wait) rep_wait=150;
			key_state = wait_rep;
			TimerDebounce=rep_wait;
		} else
		if( key_press && wait_rep==key_state && !TimerDebounce ) {
			key_state = rep;
		} else
		if( key_press && rep==key_state && !TimerDebounce ) {
			TimerDebounce = rep_time;
			if(rep_proc_callback) rep_proc_callback(dt);						/* KEY_REP */
		}
	}

	if( key_state>=wait_rep && !key_press ) {
		key_state = idle;
		last_key = 0;
	}
}

void ustawienia(TDATETIME *dt)
{
	Timer2=500;
	pozycja=0;
	uint8_t i=0;
		//SuperDebounce(&PINB,KEY1,2,100,nastepne,NULL);

	while(Timer2)
	{
			register_push_proc_callback(rtc_dodaj_godzine);
			register_rep_proc_callback(rtc_dodaj_godzine);
			while(pozycja == 0 && Timer2)
			{
			if(!Timer1)
			{
					if(i%2==0)
					{
						cy1= dt->hh/10;
						cy2= dt->hh%10;
					}
					else
					{
						cy1= PUSTY;
						cy2= PUSTY;
					}
						cy3= KRESKA;
						cy4= dt->mm/10;
						cy5= dt->mm%10;
						cy6= KRESKA;
						cy7= dt->ss/10;
						cy8= dt->ss%10;

				i++;
				Timer1 = 20;

			}

			SuperDebounce(&PINB,KEY2,REP_TIME,REP_WAIT, push_proc_callback, rep_proc_callback, &datetime);
			SuperDebounce(&PINB,KEY1,REP_TIME,REP_WAIT, nastepne, 0 , &datetime);
			}

			register_push_proc_callback(rtc_dodaj_minute);
			register_rep_proc_callback(rtc_dodaj_minute);
			while(pozycja == 1 && Timer2)
				{
				if(!Timer1)
				{
						if(i%2==0)
						{
							cy4= dt->mm/10;
							cy5= dt->mm%10;
						}
						else
						{
							cy4= PUSTY;
							cy5= PUSTY;
						}
							cy3= KRESKA;
							cy1= dt->hh/10;
							cy2= dt->hh%10;
							cy6= KRESKA;
							cy7= dt->ss/10;
							cy8= dt->ss%10;

					i++;
					Timer1 = 20;

				}
				SuperDebounce(&PINB,KEY2,REP_TIME,REP_WAIT, push_proc_callback, rep_proc_callback, &datetime);
				SuperDebounce(&PINB,KEY1,REP_TIME,REP_WAIT,nastepne,0, &datetime);
				}

			register_push_proc_callback(rtc_dodaj_dzien);
			register_rep_proc_callback(rtc_dodaj_dzien);
			while(pozycja == 2 && Timer2)
				{
				if(!Timer1)
				{
						if(i%2==0)
						{
							cy1= dt->DD/10;
							cy2= z_kropka(dt->DD%10);
						}
						else
						{
							cy1= PUSTY;
							cy2= PUSTY;
						}
							cy3= dt->MM/10;
							cy4= z_kropka(dt->MM%10);
							cy5= dt->YY/1000;
							cy6= (dt->YY-cy5*1000)/100;
							cy7= (dt->YY-cy5*1000-cy6*100)/10;
							cy8= dt->YY%10;

					i++;
					Timer1 = 20;

				}
				SuperDebounce(&PINB,KEY2,REP_TIME,REP_WAIT, push_proc_callback, rep_proc_callback, &datetime);
				SuperDebounce(&PINB,KEY1,REP_TIME,REP_WAIT,nastepne,0, &datetime);
				}

			register_push_proc_callback(rtc_dodaj_miesiac);
			register_rep_proc_callback(rtc_dodaj_miesiac);
			while(pozycja == 3 && Timer2)
				{
				if(!Timer1)
				{
						if(i%2==0)
						{
							cy3= dt->MM/10;
							cy4= z_kropka(dt->MM%10);
						}
						else
						{
							cy3= PUSTY;
							cy4= PUSTY;
						}
							cy1= dt->DD/10;
							cy2= z_kropka(dt->DD%10);
							cy5= dt->YY/1000;
							cy6= (dt->YY-cy5*1000)/100;
							cy7= (dt->YY-cy5*1000-cy6*100)/10;
							cy8= dt->YY%10;

					i++;
					Timer1 = 20;

				}
				SuperDebounce(&PINB,KEY2,REP_TIME,REP_WAIT, push_proc_callback, rep_proc_callback, &datetime);
				SuperDebounce(&PINB,KEY1,REP_TIME,REP_WAIT,nastepne,0, &datetime);
				}

			register_push_proc_callback(rtc_dodaj_rok);
			register_rep_proc_callback(rtc_dodaj_rok);
			while(pozycja == 4 && Timer2)
				{
				if(!Timer1)
				{
						if(i%2==0)
						{
							cy5= dt->YY/1000;
							cy6= (dt->YY-cy5*1000)/100;
							cy7= (dt->YY-cy5*1000-cy6*100)/10;
							cy8= dt->YY%10;
						}
						else
						{
							cy5= PUSTY;
							cy6= PUSTY;
							cy7= PUSTY;
							cy8= PUSTY;
						}
							cy1= dt->DD/10;
							cy2= z_kropka(dt->DD%10);
							cy3= dt->MM/10;
							cy4= z_kropka(dt->MM%10);
					i++;
					Timer1 = 20;
				}
				SuperDebounce(&PINB,KEY2,REP_TIME,REP_WAIT, push_proc_callback, rep_proc_callback, &datetime);
				SuperDebounce(&PINB,KEY1,REP_TIME,REP_WAIT,nastepne,0, &datetime);
				}
	}
	Timer2 = 700;
}

void nastepne()
{
	if(pozycja<4)
	pozycja++;
	else pozycja=0;
	Timer2=500;
}

void czas( TDATETIME *dt)
{
	if(datetime.ss%2)
	{
		cy1= dt->hh/10;
		cy2= dt->hh%10;
		cy3= KRESKA;
		cy4= dt->mm/10;
		cy5= dt->mm%10;
		cy6= KRESKA;
		cy7= dt->ss/10;
		cy8= dt->ss%10;
	}
	else
	{
		cy1= dt->hh/10;
		cy2= dt->hh%10;
		cy3= PUSTY;
		cy4= dt->mm/10;
		cy5= dt->mm%10;
		cy6= PUSTY;
		cy7= dt->ss/10;
		cy8= dt->ss%10;
	}
}

void data(TDATETIME *dt)
{
	cy1= dt->DD/10;
	cy2= z_kropka(dt->DD%10);
	cy3= dt->MM/10;
	cy4= z_kropka(dt->MM%10);
	cy5= dt->YY/1000;
	cy6= (dt->YY-cy5*1000)/100;
	cy7= (dt->YY-cy5*1000-cy6*100)/10;
	cy8= dt->YY%10;
}

void temperatura(uint8_t cel, uint8_t cel_fract_bits)
{

	cy1= PUSTY;
	cy2= cel/10;
	cy3= z_kropka(cel%10);
	cy4= cel_fract_bits;
	cy5= PUSTY;
	cy6= 10;
	cy7= 11;
	cy8= PUSTY;

}

void huminidy( uint8_t wilgotnosc )
{
	cy1= PUSTY;
	cy3= wilgotnosc/10;
	cy4= wilgotnosc%10;
	cy2= PUSTY;
	cy5= PUSTY;
	cy6= 15;
	cy7= 12;
	cy8= PUSTY;
}

void cisnienie( uint16_t pressure)
{
	cy1= pressure/1000;
	cy2= (pressure-cy1*1000)/100;
	cy3= (pressure-cy2*100-cy1*1000)/10;
	cy4= pressure%10;
	cy5= PUSTY;
	cy6= 12;
	cy7= 13;
	cy8= 14;
}

void wyswietlaj(TDATETIME *dt)
{
	switch(menu)
				{
				case 1:
					czas(&datetime);
					break;
				case 2:
					data(&datetime);
					break;
				case 3:
					temperatura(cel, cel_fract_bits);
					break;
				case 4:
					huminidy(wilgotnosc);
					break;
				case 5:
					cisnienie((uint16_t)pressure);
					break;
				}
}

