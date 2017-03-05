/**
 * @file testing.h
 *
 * @brief This file contains definitions for testing functions
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
#ifndef _TESTING_
#define _TESTING_

#define TESTING_DELAY_SHORT CLOCK_TICKER_FAST_5MS
#define TESTING_DELAY_LONG  CLOCK_TICKER_FAST_10MS

#define getPIN(x) (*(&x - 2)) // Address Of Data Direction Register Of Port x
#define getDDR(x) (*(&x - 1)) // Address Of Input Register Of Port x
#define getPORT(x) (x)

uint8_t testing_checkIO(uint8_t r_port, uint8_t pinNo, uint8_t reference_state);
void    testing_setIO  (uint8_t r_port, uint8_t pinNo, uint8_t InputOutput, uint8_t state);
//uint8_t testing_checkADC(uint8_t adc_channel, uint16_t lowerADCvalBound, uint16_t upperADCvalBound);
//uint8_t testing_checkFreq(uint8_t ddr, uint8_t port, uint8_t pin, uint8_t pinNo, uint16_t lowerFreqBound, uint16_t upperFreqBound);


#endif
