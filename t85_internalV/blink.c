//#define F_CPU 8000000UL

#include <stdlib.h>
#include <avr/interrupt.h>
#include "serial.h"

uint16_t read_volt() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  ADMUX = _BV(MUX3) | _BV(MUX2);
  
  _delay_ms(2); // Wait for Vref to settle

  ADCSRA |= _BV(ADEN);
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring


  //uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  //uint8_t high = ADCH; // unlocks both
  //uint16_t result = (high<<8) | low;
  uint16_t result = ADCW;
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}
int main ()
{
  sbi(DDRB,  PB3);
  sbi(PORTB, PB3);
  
  sbi(DDRB,  PB4); // PB4 as output
  sbi(PORTB, PB4); // serial idle level is '1'
  serial_setup();

  while (1) {

    char buffer[20];
    uint16_t res = read_volt();
    utoa(res, buffer, 10);
    // dtostrf(temp, 6, 4, buffer);
    serial_print(buffer);
    serial_print("\n\n");
    //if (res < 8000)       serial_print("ASDF");

    _delay_ms(100);
  }
    

}