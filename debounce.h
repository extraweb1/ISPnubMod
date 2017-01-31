/************************************************************************/
/*                                                                      */
/*                      Debouncing 8 Keys                               */
/*                      Sampling 4 Times                                */
/*                      With Repeat Function                            */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/************************************************************************/

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "hal.h"

#define KEY_PIN         IO_SWITCH_PIN

#define REPEAT_MASK     0//(1<<KEY1 | 1<<KEY2)      	// repeat keys
#define REPEAT_START    50                        		// after 500ms
#define REPEAT_NEXT     20                        		// every 200ms

volatile uint8_t key_state;                                // debounced and inverted key state:
															// bit = 1: key pressed
volatile uint8_t key_press;                                // key press detect
 
volatile uint8_t key_rpt;                                  // key long press and repeat


void tickDebounce(void);
uint8_t get_key_press( uint8_t key_mask );
uint8_t get_key_rpt( uint8_t key_mask );
uint8_t get_key_state( uint8_t key_mask );
uint8_t get_key_short( uint8_t key_mask );
uint8_t get_key_long( uint8_t key_mask );