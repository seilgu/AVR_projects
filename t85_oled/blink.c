//#define F_CPU 8000000UL

//#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
//#include "../usitwix/usitwix/usitwim.h"
#include "ssd1306xled/ssd1306xled.h"

#define MLX90614_I2CADDR 0x5A
#define MLX90614_TOBJ1 0x07

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) // clear bit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))  // set bit


#define TEMP_TO_INTREP(x) ( 50*x + 13657 )
#define TESTING_DELAY 500

int main ()
{
	sbi(DDRB,  PB3);
	sbi(PORTB, PB3);

	//usitwim_init();

	ssd1306_init();
	ssd1306_clear();
	ssd1306_setpos(0, 0);
	for (uint16_t i = 0; i < 128 * 9; i++) {
		if (i%5 == 0) ssd1306_byte(0xff);
		else ssd1306_byte(0x00);
	}
	_delay_ms(TESTING_DELAY);
	ssd1306_clear();
	_delay_ms(TESTING_DELAY);

	/*uint8_t i2c_receive_buffer[4];
	uint8_t i2c_receive_buffer_len = 4;

	uint8_t code = 0;
	
	while (1) {
		i2c_receive_buffer[1] = MLX90614_TOBJ1;  //Internal address
		code = usitwim_mem_read(MLX90614_I2CADDR, i2c_receive_buffer, i2c_receive_buffer_len);

		uint16_t ret = 0;
		ret = i2c_receive_buffer[1];
		ret |= ((uint16_t)i2c_receive_buffer[2]) << 8;
		//float temp = ret*.02 - 273.15;

		if (ret > TEMP_TO_INTREP(22)) {	
			sbi(PORTB, PB3);
		} else {
			cbi(PORTB, PB3);
		}
		
		// char buffer[20];
		// dtostrf(temp, 6, 4, buffer);
		// serial_print(buffer);

		_delay_ms(100);
	}*/
		


}
