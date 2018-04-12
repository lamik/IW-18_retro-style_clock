/*                  e-gadget.header
 * MPL115A2.c
 *
 *  Created on: 01-03-2014
 *    Modyfied: 2015-05-21 09:56:10
 *      Author: Mateusz Salamon
 *
 *          MCU: ATmega32
 *        F_CPU: 16 000 000 Hz
 *
 */

#include "MPL115A2.h"

void MPL115A2_start_conversion()
{
	TWI_start();
	TWI_write(MPL115A2_ADDRESS_WRITE);
	TWI_write(MPL1152A_CONVERT);
	//TWI_write(0x00);
	TWI_stop();
}

void MPL115A2_get_pressure_and_temperature()
{
	  uint16_t 	press, temp;
	  float     pressureComp;

	  //Najpierw trzeba puscic konwersje i poczekac ok 3sek
	  TWI_start();

	  TWI_write(MPL115A2_ADDRESS_WRITE);
	  TWI_write(MPL1152A_READ_PRESSURE_MSB); //adres MSB cisnienia
	  TWI_stop();

	  TWI_start();
	  TWI_start();
	  TWI_write(MPL115A2_ADDRESS_READ);

	  press = (( (uint16_t) TWI_read(1) << 8) | TWI_read(1)) >> 6;
	  temp = (( (uint16_t) TWI_read(1) << 8) | TWI_read(0)) >> 6;
	  TWI_stop();

	  //Przeliczenie cisnienia
	  pressureComp = a0 + (b1 + c12 * temp ) * press + b2 * temp;

	  //Zwracane wartosci
	  pressure = ((65.0 / 1023.0) * pressureComp) + 50.0;        // kPa
	  pressure *= 10.0;											//hPa
	  temperature = ((float) temp - 498.0) / -5.35 +25.0;           // C
}


void MPL115A2_read_coeffs()
{
	int16_t a0coeff;
	int16_t b1coeff;
	int16_t b2coeff;
	int16_t c12coeff;

	TWI_start();
	TWI_write(MPL115A2_ADDRESS_WRITE);
	TWI_write(MPL1152A_READ_COEFFICIENT);
	TWI_stop();
	TWI_start();
	TWI_start();
	TWI_write(MPL115A2_ADDRESS_READ);

	a0coeff = (( (uint16_t) TWI_read(1) << 8) | TWI_read(1));
	b1coeff = (( (uint16_t) TWI_read(1) << 8) | TWI_read(1));
	b2coeff = (( (uint16_t) TWI_read(1) << 8) | TWI_read(1));
	c12coeff = (( (uint16_t) (TWI_read(1) << 8) | TWI_read(0))) >> 2;

	TWI_stop();

	a0 = (float)a0coeff ;/// 8;
	b1 = (float)b1coeff ;/// 8192;
	b2 = (float)b2coeff ; /// 16384;
	c12 = (float)c12coeff;
	//c12 /= 4194304.0;
}


