/*                  e-gadget.header
 * MPL115A2.h
 *
 *  Created on: 01-03-2014
 *    Modyfied: 2015-05-21 09:56:10
 *      Author: Mateusz Salamon
 *
 *          MCU: ATmega32
 *        F_CPU: 16 000 000 Hz
 *
 */

#ifndef MPL1152A_H_
#define MPL1152A_H_

#include <avr/io.h>

#include "../I2C_TWI/i2c_twi.h"

#define MPL115A2_ADDRESS_READ  0xC1
#define MPL115A2_ADDRESS_WRITE 0xC0
#define MPL1152A_CONVERT 0x12
#define MPL1152A_READ_PRESSURE_MSB 0x00 //8 bits
#define MPL1152A_READ_PRESSURE_LSB 0x01 //2 bits
#define MPL1152A_READ_TEMPERATURE_MSB 0x02 //8 bits
#define MPL1152A_READ_TEMPERATURE_LSB 0x03 //2 bits
#define MPL1152A_READ_COEFFICIENT 0x04 //8 bytes

float pressure;
float temperature;


//coefficients
float a0;
float b1;
float b2;
float c12;

void MPL115A2_start_conversion();
void MPL115A2_get_pressure_and_temperature();
void MPL115A2_read_coeffs();

#endif /* MPL1152A_H_ */

