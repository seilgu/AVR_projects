
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../usitwix/usitwix/usitwim.h"
#include "ssd1306xled/ssd1306xled.h"
#include "ssd1306xled/font8x16.h"
//#include "ssd1306xled/ssd1306xledtx.h"

#define MLX90614_I2CADDR 0x5A
#define MLX90614_TOBJ1 0x07

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) // clear bit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))  // set bit


#define TEMP_TO_INTREP(x) ( 50*x + 13657 )
#define TESTING_DELAY 100

void ssd1306_string_font8x16xy(uint8_t x, uint8_t y, const char s[], uint8_t len) {
	uint8_t ch, j = 0;
	while (j < len) {
		ch = s[j] - 32; // Convert ASCII code to font data index.
		if (x > 120) {
			x = 0;    // Go to the next line.
			y++;
		}
		ssd1306_setpos(x, y);
		ssd1306_send_data_start();
		for (uint8_t i = 0; i < 8; i++) {
			ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font8x16data[ch * 16 + i]));
		}
		ssd1306_send_data_stop();
		_delay_ms(1);
		ssd1306_setpos(x, y + 1);
		ssd1306_send_data_start();
		for (uint8_t i = 0; i < 8; i++) {
			ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font8x16data[ch * 16 + i + 8]));
		}
		ssd1306_send_data_stop();
		_delay_ms(1);
		x += 8;
		j++;
	}
}

int main ()
{
	//sbi(DDRB,  PB3);
	//sbi(PORTB, PB3);

	usitwim_init();
	ssd1306_init();
	_delay_ms(10);

	uint8_t i2c_receive_buffer[4];
	uint8_t i2c_receive_buffer_len = 4;

	ssd1306_clear();
	ssd1306_setpos(0,0);
	for (uint16_t i = 0; i < 128 * 9; i++) {
		ssd1306_byte(0x00);
	}
	_delay_ms(100);

	while (1) {
		i2c_receive_buffer[1] = MLX90614_TOBJ1;  //Internal address
		usitwim_mem_read(MLX90614_I2CADDR, i2c_receive_buffer, i2c_receive_buffer_len);

		uint16_t ret = 0;
		ret = i2c_receive_buffer[1];
		ret |= ((uint16_t)i2c_receive_buffer[2]) << 8;
		uint16_t tempx100 = ret*2-27315;
		//float temp = ret*.02 - 273.15;
		char buffer[20];
		itoa(tempx100, buffer, 10);
		//dtostrf(temp, 2, 3, buffer);

		ssd1306_string_font8x16xy(0, 0, "temp:", 5);
		ssd1306_string_font8x16xy(5*8, 0, buffer, 2);
		ssd1306_string_font8x16xy(7*8, 0, ".", 1);
		ssd1306_string_font8x16xy(8*8, 0, buffer+2, 2);
		_delay_ms(100);
		//if (ret > TEMP_TO_INTREP(21)) {	
		// if (temp > 21.0) {	
		// 	ssd1306_clear();
		// 	ssd1306_setpos(0, 0);
		// 	ssd1306_string_font8x16xy(0, 0, dtostrf(temp));
		// 	// for (uint16_t i = 0; i < 128 * 8; i++) {
		// 	//  	if (i%5 == 0) ssd1306_byte(0xff);
		// 	// 	else ssd1306_byte(0x00);
		// 	// }
		// 	_delay_ms(TESTING_DELAY);
			
		// 	//sbi(PORTB, PB3);
		// } else {
		// 	ssd1306_clear();
		// 	_delay_ms(TESTING_DELAY);
		// 	//cbi(PORTB, PB3);
		// }
		
		// char buffer[20];
		// dtostrf(temp, 6, 4, buffer);
		// serial_print(buffer);

		//_delay_ms(100);
	}
		


}
