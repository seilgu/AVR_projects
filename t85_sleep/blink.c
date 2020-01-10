//#define F_CPU 8000000UL
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) // clear bit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))  // set bit

void setup_watchdog() 
{
  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  
  WDTCR |= (1<<WDCE) | (1<< WDE);
  
  //WDTCR = 6; // 1 sec
  WDTCR = (1<<WDP3); // 4 sec
  sbi(WDTCR, WDIE);
}


// system wakes up when watchdog is timed out
void system_sleep() 
{
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF
  setup_watchdog();                   // approximately 8 seconds sleep
 
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
  sei();                               // Enable the Interrupts so the wdt can wake us up

  sleep_mode();                        // System sleeps here

  sleep_disable();                     // System continues execution here when watchdog timed out 
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
}


/*** ISR ***/

// compare match interrupt service for OCR0A
// call every 103us
extern "C" void TIM0_COMPA_vect(void) __attribute__ ((signal));
extern "C" void WDT_vect(void) __attribute__ ((signal));
void WDT_vect(void) {
}
void TIM0_COMPA_vect(void) {
	PORTB ^= (1 << PB3);
}

int main ()
{

 	/*sbi(TCCR0A, WGM00);
 	sbi(TCCR0B, WGM02);
	sbi(TCCR0B, CS01);
	 // compare register A at 103 us
	OCR0A = 113;
	// interrupt on compare match OCROA == TCNT0
	sbi(TIMSK, OCIE0A);
	sei();*/


	while (1) {
    //sbi(PORTB, PB3);
    sbi(DDRB,  PB3);
    sbi(PORTB, PB3);
    _delay_ms(4000);
    cbi(PORTB, PB3);
    cbi(PORTB, PB3);
    //_delay_ms(1000);
    //cbi(PORTB, PB3);
    system_sleep();
	}
}