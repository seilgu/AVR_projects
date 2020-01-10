//#define F_CPU 8000000UL

//#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../usitwix/usitwix/usitwim.h"

#define MLX90614_I2CADDR 0x5A
#define MLX90614_TOBJ1 0x07

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) // clear bit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))  // set bit

/* some vars */
volatile uint64_t _millis    = 0;
volatile uint16_t _1000us    = 0;
uint64_t old_millis = 0;

// must be volatile (change and test in main and ISR)
volatile uint8_t tx_buzy = 0;
volatile uint8_t bit_index;
volatile uint8_t _tx_buffer; 

/*** ISR ***/

// compare match interrupt service for OCR0A
// call every 103us
extern "C" void TIM0_COMPA_vect(void) __attribute__ ((signal));
void TIM0_COMPA_vect(void) {
//ISR(TIM0_COMPA_vect) { 
  // software UART
  // send data
  if (tx_buzy) {
    if (bit_index == 0) {
      // start bit (= 0)
      cbi(PORTB, PB4);
    } else if (bit_index <=8) {
      // LSB to MSB
      if (_tx_buffer & 1) {
        sbi(PORTB, PB4);
      } else {
        cbi(PORTB, PB4);
      }
      _tx_buffer >>= 1;        
    } else if (bit_index >= 9) {
      // stop bit (= 1)
      sbi(PORTB, PB4);
      tx_buzy = 0;
    }
    bit_index++;
  }
  // millis update
  _1000us += 103;
  while (_1000us > 1000) {
    _millis++;
    _1000us -= 1000;
  }
}


void serial_write(uint8_t tx_byte) {
  while(tx_buzy);
  bit_index  = 0;
  _tx_buffer = tx_byte;
  tx_buzy = 1;
}

void serial_print_binary(uint8_t val) {
	static const char* digits = "0123456789ABCDEF";
	//uint8_t i;
	serial_write( digits[(val>>4)] );
	serial_write( digits[(val & 0x0F)] );
	
	serial_write('\n');
}
void serial_print(const char *str) {
  uint8_t i;
  for (i = 0; str[i] != 0; i++) {
    serial_write(str[i]);
  }
}

#define TEMP_TO_INTREP(x) ( 50*x + 13657 )

int main ()
{
	sbi(DDRB,  PB3);
	sbi(PORTB, PB3);
	
	sbi(DDRB,  PB4); // PB4 as output
 	sbi(PORTB, PB4); // serial idle level is '1'

 	sbi(TCCR0A, WGM01); // prescaler : clk/8 (1 tic = 1us for 8MHz clock)
	sbi(TCCR0B, CS01); // compare register A at 103 us
	OCR0A = 103;// interrupt on compare match OCROA == TCNT0
	sbi(TIMSK, OCIE0A);// Enable global interrupts
	sei();

	usitwim_init();

	uint8_t i2c_receive_buffer[4];
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
	}
		

	/*while (1) {
		serial_print("hello\n");
		_delay_ms(100);
	}*/

	// while (1) {
	// 	/*serial_print_binary(i2c_transmit_buffer[0]);
	// 	serial_print_binary(i2c_transmit_buffer[1]);
	// 	serial_print_binary(i2c_transmit_buffer[3]);
	// 	serial_print_binary(i2c_transmit_buffer[4]);
	// 	serial_print_binary(i2c_transmit_buffer[5]);
	// 	serial_print_binary(i2c_transmit_buffer[6]);*/

	// 	serial_print_binary(i2c_receive_buffer[0] >> 1);
	// 	serial_print_binary(i2c_receive_buffer[1]);
	// 	serial_print_binary(i2c_receive_buffer[2]);
	// 	serial_print_binary(i2c_receive_buffer[3]);
	// 	//serial_print_binary(i2c_receive_buffer[4]);
	// 	//serial_print_binary(i2c_receive_buffer[5]);
	// 	PORTB &= ~(1 << PB3);
	// 	_delay_ms(1000);
	// 	return 0;
	// }


	// uint16_t ret = 0;
	// ret = i2c_receive_buffer[1];
	// ret |= ((uint16_t)i2c_receive_buffer[2]) << 8;

	// float temp = ret*.02 - 273.15;
	// //char buffer[20];
	// //dtostrf(temp, 6, 4, buffer);
	// //serial_print(buffer);
	// if ((temp > 15 && temp < 25)) {	
	// 	while(1) 
	// 	{
	// 		PORTB ^= (1 << PB3);
	// 		_delay_ms(1000);
			
	// 	}
	// }
	//Transmit the I2C message
	//USI_I2C_Master_Transceiver_Start(i2c_transmit_buffer, i2c_transmit_buffer_len);

	//USI_I2C_Master_Start_Transmission(i2c_transmit_buffer, i2c_transmit_buffer_size);


}
