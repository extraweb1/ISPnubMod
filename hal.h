/**
 * @file hal.h
 *
 * @brief This file contains definitions for hardware abstraction
 *
 * @author Thomas Fischl
 * @copyright (c) 2013 Thomas Fischl
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

// ******************************** ATmega1284P ***********************************
#if defined (__AVR_ATmega1284P__)

#define IO_LED_GREEN	PD4		// at Port D
#define IO_LED_RED		PC3		// at Port C
#define IO_SWITCH		PD3		// at Port D (INT1)
#define IO_EXT_SWITCH	PD2		// at Port D (INT0)
#define IO_BUZZER		PA0		// at Port A
#define IO_SWITCH_PIN	PIND	// all switches have to be on same port to be debounced

#define hal_setLEDgreen(x)	PORTD =	(PORTD & ~(1 << IO_LED_GREEN))	| ((!x) << IO_LED_GREEN)										// inverses logic: setLEDx(1) turns it on
#define hal_setLEDred(x)	PORTC =	(PORTC & ~(1 << IO_LED_RED))	| ((!x) << IO_LED_RED)
#define hal_setBuzzer(x)	PORTA = (PORTA & ~(1 << IO_BUZZER))		| ((!x) << IO_BUZZER)
#define hal_init()			DDRD = (1 << IO_LED_GREEN); DDRC = (1 << IO_LED_RED); DDRA = (1 << IO_BUZZER); PORTD = ( (1<<IO_SWITCH) | (1<<IO_EXT_SWITCH) );	//LEDs & Buzzer: Output; Switch: Enable Pullup
#define hal_enableINT0()	EIMSK |= (1<<INT0);												//Enable INT0, fire on low level (this is the only detectable state in powerdown)
#define hal_disableINT0()	EIMSK &= ~(1<<INT0);
#define hal_enableINT1()	EIMSK |= (1<<INT1);												//Enable INT1, fire on low level (this is the only detectable state in powerdown)
#define hal_disableINT1()	EIMSK &= ~(1<<INT1);

#define DEFINE_DATAPOINTER uint32_t scriptdata_p = FAR(scriptdata);
#define flash_readbyte(x) pgm_read_byte_far(x)

#define	ISP_OUT   PORTB
#define ISP_IN    PINB
#define ISP_DDR   DDRB
#define ISP_RST   PB4
#define ISP_MOSI  PB5
#define ISP_MISO  PB6
#define ISP_SCK   PB7

#define TCCR0 TCCR0B
#define TIMSK TIMSK0

// ************************ ATmega328 on Arduino Uno *****************************
#elif defined (__AVR_ATmega328P__)

#define IO_LED_GREEN	PC2		// Arduino Uno: A2
#define IO_LED_RED		PC1		// Arduino Uno: A1
#define IO_SWITCH		PD3		// Arduino Uno: D3 (INT1)
#define IO_EXT_SWITCH	PD2		// Arduino Uno: D2 (INT0)
#define IO_BUZZER		PC0		// Arduino Uno: A0
#define IO_SWITCH_PIN	PIND	// all switches have to be on same port to be debounced

#define hal_setLEDgreen(x)	PORTC =	(PORTC & ~(1 << IO_LED_GREEN))	| ((x) << IO_LED_GREEN)
#define hal_setLEDred(x)	PORTC =	(PORTC & ~(1 << IO_LED_RED))	| ((x) << IO_LED_RED)
#define hal_setBuzzer(x)	PORTC = (PORTC & ~(1 << IO_BUZZER))		| ((x) << IO_BUZZER)
#define hal_init()			DDRC = ( (1 << IO_LED_GREEN) | (1 << IO_LED_RED) | (1 << IO_BUZZER) ); PORTD = ( (1<<IO_SWITCH) | (1<<IO_EXT_SWITCH) );	//LEDs & Buzzer: Output; Switch: Enable Pullup
#define hal_enableINT0()	EIMSK |= (1<<INT0);												//Enable INT0, fire on low level (this is the only detectable state in powerdown)
#define hal_disableINT0()	EIMSK &= ~(1<<INT0);
#define hal_enableINT1()	EIMSK |= (1<<INT1);												//Enable INT1, fire on low level (this is the only detectable state in powerdown)
#define hal_disableINT1()	EIMSK &= ~(1<<INT1);

#define DEFINE_DATAPOINTER uint16_t scriptdata_p = (uint16_t)scriptdata
#define flash_readbyte(x) pgm_read_byte(x)

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
