//#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) // clear bit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))  // set bit

/*** ISR ***/

// compare match interrupt service for OCR0A
// call every 103us
extern "C" void TIM0_COMPA_vect(void) __attribute__ ((signal));
void TIM0_COMPA_vect(void) {
	PORTB ^= (1 << PB3);
}

int main ()
{
	sbi(DDRB,  PB3);
	sbi(PORTB, PB3);

/*
	  // call ISR(TIM0_COMPA_vect) every 103us (for 9600 bauds)
  // set CTC mode : clear timer on comapre match
  // -> reset TCNTO (timer 0) when TCNTO == OCR0A
  sbi(TCCR0A, WGM01);
  // prescaler : clk/8 (1 tic = 1us for 8MHz clock)
  sbi(TCCR0B, CS01);
  // compare register A at 103 us
  OCR0A = 103;
  // interrupt on compare match OCROA == TCNT0
  sbi(TIMSK, OCIE0A);
  // Enable global interrupts
  sei();
  	while (1) {
  		_delay_ms(1);
  	}*/

 	//sbi(TCCR0A, WGM01);
 	//sbi(TCCR0A, COM0A0);
 	
 	//sbi(TCCR0A, COM0A0);
 	//sbi(TCCR0A, COM0A1);

 	sbi(TCCR0A, WGM00);

 	sbi(TCCR0B, WGM02);
	sbi(TCCR0B, CS01);
	 // compare register A at 103 us
	OCR0A = 113;
	// interrupt on compare match OCROA == TCNT0
	sbi(TIMSK, OCIE0A);
	sei();

	while (1) {

	}
}