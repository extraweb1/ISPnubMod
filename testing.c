/**
 * @file testing.c
 *
 * @brief This file contains testing functions
 *
 * @author Michael Gröne
 * @copyright (c) 2017 Michael Gröne
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
#include <inttypes.h>
#include <avr/io.h>
#include "clock.h"
#include "hal.h"
#include "testing.h"


/**
 * @brief Check for defined state on pin at port
 * @param ddr DDR on ISPnub for desired port/pin
 * @param port Port on ISPnub to check the state
 * @param pin Pin@Port on ISPnub to check the state
 * @param reference_state Expected state
 * @retval 0 state other than expected
 * @retval 1 state OK
 */
uint8_t testing_checkIO(uint8_t ddr, uint8_t port, uint8_t pin, uint8_t reference_state) {
	
	//make sure its input and has internal pullups off
	ddr &= ~(1 << pin);
	port &= ~(1 << pin);
	
	//read state 1st time
	uint8_t read_state=port & (1<<pin);
	
	//short delay
	clock_delayFast(TESTING_DELAY_SHORT);
	
	//reread
	if(read_state != port & (1<<pin)) {
		return 0; 	//pin not on defined state
	}
	
	if(read_state == reference_state)
		return 1;
	else
		return 0;
	
}

/**
 * @brief Set state on pin at port
 * @param ddr DDR on ISPnub for desired port/pin
 * @param port Port on ISPnub to set the state
 * @param pin Pin@Port on ISPnub to set the state
 * @param state State to set to
 */
void testing_setIO(uint8_t ddr, uint8_t port, uint8_t pin, uint8_t state);

/**
 * @brief Check for frequency in a range between lowerFreqBound and upperFreqBound
 * @param port Port on ISPnub to check
 * @param pin Pin@Port on ISPnub to check
 * @param lowerFreqBound Lower bound of expected frequency
 * @param upperFreqBound Upper bound of expected frequency
 * @retval 0 state other than expected
 * @retval 1 state OK
 */
uint8_t testing_checkFreq(uint8_t port, uint8_t pin, uint16_t lowerFreqBound, uint16_t upperFreqBound);

