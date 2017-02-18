/**
 * @file hal.h
 *
 * @brief This file contains definitions for hardware abstraction
 *
 * @author Thomas Fischl, Michael Gröne
 * @copyright (c) 2013, 2017 Thomas Fischl, Michael Gröne
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef _HAL_
#define _HAL_

// ******************************** ATmega1284(P) / ATmega16(A) / ATmega32 / ATmega644(A/P) *******************************
#if defined (__AVR_ATmega1284__) || defined (__AVR_ATmega1284P__) || defined (__AVR_ATmega16__) || defined (__AVR_ATmega16A__) || defined (__AVR_ATmega32__) || defined (__AVR_ATmega644__) || defined (__AVR_ATmega644A__) || defined (__AVR_ATmega644P__)

// all have to run at 8MHz internal clock

#define IO_LED_GREEN	PD4		// at Port D
#define IO_LED_YELLOW	PD5		// at Port D
#define IO_LED_RED		PC3		// at Port C
#define IO_BUZZER		PA0		// at Port A
#define IO_SWITCH		PD3		// at Port D (INT1)
#define IO_EXT_SWITCH	PD2		// at Port D (INT0)
#define IO_SWITCH_PIN	PIND	// all switches have to be on same port to be debounced

#define hal_setLEDgreen(x)	PORTD =	(PORTD & ~(1 << IO_LED_GREEN))	| ((x) << IO_LED_GREEN)
#define hal_setLEDyellow(x)	PORTD =	(PORTD & ~(1 << IO_LED_YELLOW))	| ((x) << IO_LED_YELLOW)
#define hal_setLEDred(x)	PORTC =	(PORTC & ~(1 << IO_LED_RED))	| ((x) << IO_LED_RED)

#define hal_setBuzzer(x)	PORTA = (PORTA & ~(1 << IO_BUZZER))		| ((x) << IO_BUZZER)
#define hal_init()			DDRD = (1 << IO_LED_GREEN) | (1 << IO_LED_YELLOW); DDRC = (1 << IO_LED_RED); DDRA = (1 << IO_BUZZER); PORTD = ( (1<<IO_SWITCH) | (1<<IO_EXT_SWITCH) );	//LEDs & Buzzer: Output; Switch: Enable Pullup
#define hal_enableINT0()	EIMSK |= (1<<INT0);												//Enable INT0, fire on low level (this is the only detectable state in powerdown)
#define hal_disableINT0()	EIMSK &= ~(1<<INT0);
#define hal_enableINT1()	EIMSK |= (1<<INT1);												//Enable INT1, fire on low level (this is the only detectable state in powerdown)
#define hal_disableINT1()	EIMSK &= ~(1<<INT1);

#define	ISP_OUT   PORTB
#define ISP_IN    PINB
#define ISP_DDR   DDRB
#define ISP_RST   PB4
#define ISP_MOSI  PB5
#define ISP_MISO  PB6
#define ISP_SCK   PB7

#define TCCR0 TCCR0B
#define TIMSK TIMSK0

// ************************ ATmega328@16MHz in Arduino Uno *****************************
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)

// Arduino Uno has 16MHz Crystal -> scaled by 2 to have correct timing.
// Attention: Firmware works only if flashed via ISP (without bootloader) onto Arduino.

#if COMPATIBILITY_MODE_ISPNUB_ORIGINAL == 1
	#error COMPATIBILITY_MODE not supported for ATmega328
#endif

#define IO_LED_GREEN	PC3		// Arduino Uno: A3
#define IO_LED_YELLOW	PC2		// Arduino Uno: A2
#define IO_LED_RED		PC1		// Arduino Uno: A1
#define IO_BUZZER		PC0		// Arduino Uno: A0
#define IO_SWITCH		PD3		// Arduino Uno: D3 (INT1)
#define IO_EXT_SWITCH	PD2		// Arduino Uno: D2 (INT0)
#define IO_SWITCH_PIN	PIND	// all switches have to be on same port to be debounced

#define hal_setLEDgreen(x)	PORTC =	(PORTC & ~(1 << IO_LED_GREEN))	| ((x) << IO_LED_GREEN)
#define hal_setLEDyellow(x)	PORTC =	(PORTC & ~(1 << IO_LED_YELLOW))	| ((x) << IO_LED_YELLOW)
#define hal_setLEDred(x)	PORTC =	(PORTC & ~(1 << IO_LED_RED))	| ((x) << IO_LED_RED)
#define hal_setBuzzer(x)	PORTC = (PORTC & ~(1 << IO_BUZZER))		| ((x) << IO_BUZZER)
#define hal_init()			clock_prescale_set(2); DDRC = ( (1 << IO_LED_GREEN) | (1 << IO_LED_YELLOW) | (1 << IO_LED_RED) | (1 << IO_BUZZER) ); PORTD = ( (1<<IO_SWITCH) | (1<<IO_EXT_SWITCH) );	//LEDs & Buzzer: Output; Switch: Enable Pullup
#define hal_enableINT0()	EIMSK |= (1<<INT0);												//Enable INT0, fire on low level (this is the only detectable state in powerdown)
#define hal_disableINT0()	EIMSK &= ~(1<<INT0);
#define hal_enableINT1()	EIMSK |= (1<<INT1);												//Enable INT1, fire on low level (this is the only detectable state in powerdown)
#define hal_disableINT1()	EIMSK &= ~(1<<INT1);

#define	ISP_OUT   PORTB
#define ISP_IN    PINB
#define ISP_DDR   DDRB
#define ISP_RST   PB2		// Arduino Uno: D10
#define ISP_MOSI  PB3		// Arduino Uno: D11
#define ISP_MISO  PB4		// Arduino Uno: D12
#define ISP_SCK   PB5		// Arduino Uno: D13

#define TCCR0 TCCR0B
#define TIMSK TIMSK0


// ****************************** unknown device *********************************
#else 
  #error "MCU not supported by HAL"
#endif



#if defined (__AVR_ATmega1284__) || defined (__AVR_ATmega1284P__)
	//for AVR with flash > 64kb
	#define DEFINE_DATAPOINTER uint32_t scriptdata_p = FAR(scriptdata);
	#define flash_readbyte(x) pgm_read_byte_far(x)
#else
	//for AVR with flash < 64kb
	#define DEFINE_DATAPOINTER uint16_t scriptdata_p = (uint16_t)scriptdata
	#define flash_readbyte(x) pgm_read_byte(x)
#endif


/**
 * @brief Macro to access strings defined in PROGMEM above 64kB
 */
#define FAR(var)                     \
({ uint_farptr_t tmp;                \
   __asm__ __volatile__(             \
       "ldi    %A0, lo8(%1)"  "\n\t" \
       "ldi    %B0, hi8(%1)"  "\n\t" \
       "ldi    %C0, hh8(%1)"  "\n\t" \
       : "=d" (tmp)                  \
       : "p"  (&(var)));             \
   tmp;                              \
})

#endif
