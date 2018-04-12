/*                  e-gadget.header
 * common.h
 *
 *  Created on: 18-01-2014
 *    Modyfied: 2015-05-21 09:56:10
 *      Author: Mateusz Salamon
 *
 *          MCU: ATmega32
 *        F_CPU: 16 000 000 Hz
 *
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "avr/io.h"

volatile uint8_t Timer1;
volatile uint8_t Timer3, TimerDebounce; //timer programowy
volatile uint16_t Timer2; //timer programowy

#endif /* COMMON_H_ */

