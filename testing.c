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
#include <avr/sfr_defs.h>
#include "clock.h"
#include "hal.h"
#include "testing.h"


/**
 * @brief Check for defined state on pin at port
 * @param port Port on ISPnub to check the state
 * @param pinNo Pin@Port on ISPnub to check the state
 * @param reference_state Expected state
 * @retval 0 state other than expected
 * @retval 1 state OK
 */
uint8_t testing_checkIO(uint8_t r_port, uint8_t pinNo, uint8_t reference_state) {
	
	//make sure its input and has internal pullups off
	getDDR(_SFR_IO8(r_port)) &= ~(1 << pinNo);
	getPORT(_SFR_IO8(r_port)) &= ~(1 << pinNo);
	
	//read state 1st time
	uint8_t read_state=getPIN(_SFR_IO8(r_port)) & (1<<pinNo);
	
	//short delay
	clock_delayFast(TESTING_DELAY_SHORT);
	
	//reread
	if(read_state != (getPIN(_SFR_IO8(r_port)) & (1<<pinNo)) ) {
		return 0; 	//pin not on defined state
	}
	
	if(read_state == reference_state)
		return 1;
	else
		return 0;
	
}

/**
 * @brief Set state on pin at port
 * @param port Port on ISPnub to set the state
 * @param pinNo Pin@Port on ISPnub to set the state
 * @param state State to set to
 */
void testing_setIO(uint8_t r_port, uint8_t pinNo, uint8_t InputOutput, uint8_t state) {
	if(InputOutput==0)	//input
		getDDR(_SFR_IO8(r_port)) &= ~(1 << pinNo);
	else				//output
		getDDR(_SFR_IO8(r_port)) |=  (1 << pinNo);
	
	if(state==0)	//off/low
		_SFR_IO8(r_port) &= ~(1 << pinNo);
	else			//on/high
		_SFR_IO8(r_port) |=  (1 << pinNo);
	
}

/**
 * @brief Check for frequency in a range between lowerFreqBound and upperFreqBound.
 * @param ddr DDR on ISPnub for desired port/pin
 * @param port Port on ISPnub to check
 * @param pin Pin@Port on ISPnub to check
 * @param lowerFreqBound Lower bound of expected frequency
 * @param upperFreqBound Upper bound of expected frequency
 * @retval 0 state other than expected
 * @retval 1 state OK
 */
/*uint8_t testing_checkFreq(uint8_t ddr, uint8_t port, uint8_t pin, uint8_t pinNo, uint16_t lowerFreqBound, uint16_t upperFreqBound) {
	
	//make sure its input and has internal pullups on
	ddr &= ~(1 << pin);
	port &= ~(1 << pin);
	uint8_t timestamp_started = clock_getTickerSlow();
	uint8_t last_pin_state=( pin & (1<<pinNo) );
	uint16_t counter_edges=0;
	
	do {
		if(last_pin_state!=( pin & (1<<pinNo) )) {
			last_pin_state=( pin & (1<<pinNo) );
			counter_edges++;
			
		}
	} while(clock_getTickerSlowDiff(timestamp_started) < CLOCK_TICKER_SLOW_250MS);
	
	
	
}
*/
