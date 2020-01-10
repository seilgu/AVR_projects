
#include <avr/io.h>
#include <util/delay.h>

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

void serial_setup() {
  sbi(TCCR0A, WGM01); // prescaler : clk/8 (1 tic = 1us for 8MHz clock)
  sbi(TCCR0B, CS01); // compare register A at 103 us
  OCR0A = 103;// interrupt on compare match OCROA == TCNT0
  sbi(TIMSK, OCIE0A);// Enable global interrupts
  sei();
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