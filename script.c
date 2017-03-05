/**
 * @file script.c
 *
 * @brief This file contains script processing functions
 *
 * @author Thomas Fischl
 * @copyright (c) 2013-2014 Thomas Fischl
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
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "clock.h"
#include "hal.h"
#include "isp.h"
#include "counter.h"
#include "script.h"
#include "testing.h"

/**
 * @brief Pointer to script data in flash memory
 */
#ifdef DEBUG
	#warning DEBUG-MODE Choose a testdummy inline-script for debugging purposes
	
	//test decounter, take a look at eeprom afterwards
	/*unsigned char scriptdata[] SCRIPT_SECTION = {
		SCRIPT_CMD_DECCOUNTER,0,3,		// two bytes, since its counting a 16bit var
		SCRIPT_CMD_WAIT,20,
		SCRIPT_CMD_END
	};*/
	
	//simulate long-running programming task (2*255*10ms=5,1s
	/*unsigned char scriptdata[] SCRIPT_SECTION = {
		SCRIPT_CMD_WAIT,255,
		SCRIPT_CMD_WAIT,255,
		SCRIPT_CMD_END
	};*/
	
	//simulate usual behaviour
	//unsigned char scriptdata[] SCRIPT_SECTION = {SCRIPT_CMD_NOPROGRAM};
	
	//check for tiny13A signature
	unsigned char scriptdata[] SCRIPT_SECTION = { 
		/*SCRIPT_CMD_CONNECT, 7,									// sckopt=7 means SPI@8MHz/64->125kHz, see connect-routine
		SCRIPT_CMD_SPI_VERIFY, 0x30, 0x00, 0x00, 0x00, 0x1E,    // check signature byte 0x00 (0x1E = manufactured by Atmel)
		SCRIPT_CMD_SPI_VERIFY, 0x30, 0x00, 0x01, 0x00, 0x90,    // check signature byte 0x01 (0x90 = 1KB Flash memory)
		SCRIPT_CMD_SPI_VERIFY, 0x30, 0x00, 0x02, 0x00, 0x07,	// check signature byte 0x02 (0x07 = attiny13(a) device)
		SCRIPT_CMD_DISCONNECT,*/
		//SCRIPT_CMD_WAIT,100,
		SCRIPT_CMD_SETIO,      0x05, 0, 1, 0,	//PORTB: 0x05 PORTC: 0x08
		SCRIPT_CMD_WAIT,200,
		SCRIPT_CMD_SETIO,      0x05, 0, 0, 0,
		//SCRIPT_CMD_CHECKIO,    0x05, 0, 0,		//PORTB: 0x05
		SCRIPT_CMD_END
	};
	
	//check for tiny45 signature
	/*unsigned char scriptdata[] SCRIPT_SECTION = { 
		SCRIPT_CMD_CONNECT, 7,									// sckopt=7 means SPI@8MHz/64->125kHz, see connect-routine
		SCRIPT_CMD_SPI_VERIFY, 0x30, 0x00, 0x00, 0x00, 0x1E,    // check signature byte 0x00 (0x1E = manufactured by Atmel)
		SCRIPT_CMD_SPI_VERIFY, 0x30, 0x00, 0x01, 0x00, 0x92,    // check signature byte 0x01 (0x92 = 4KB Flash memory)
		SCRIPT_CMD_SPI_VERIFY, 0x30, 0x00, 0x02, 0x00, 0x06,	// check signature byte 0x06 (0x07 = attiny45 device)
		SCRIPT_CMD_DISCONNECT,
		SCRIPT_CMD_END
	};*/
#else
	unsigned char scriptdata[] SCRIPT_SECTION = {SCRIPT_CMD_NOPROGRAM}; // dummy (is overwritten by hex creator)
#endif
/**
 * @brief Execute script stored in flash memory
 * @retval 2 No Program (use ISPnubCreator to generate hex to flash targets)
 * @retval 1 Everything okay
 * @retval 0 Error occured
 */
uint8_t script_run() {

    DEFINE_DATAPOINTER;

    uint8_t cmd;
    while (1) {
        cmd = flash_readbyte(scriptdata_p++);

        uint8_t success = 0;

        switch (cmd) {

            case SCRIPT_CMD_CONNECT:
                success = isp_connect(flash_readbyte(scriptdata_p++));
                break;

            case SCRIPT_CMD_DISCONNECT:
                success = isp_disconnect();
                break;

            case SCRIPT_CMD_WAIT:
            {
                uint8_t loops = flash_readbyte(scriptdata_p++);
                while (loops--) {
                    clock_delayFast(CLOCK_TICKER_FAST_10MS);
                }
                success = 1;
            }
                break;

            case SCRIPT_CMD_SPI_SEND:
            {
                uint8_t data[4];
                uint8_t i;
                for (i = 0; i < 4; i++)
                    data[i] = flash_readbyte(scriptdata_p++);
                isp_transmit(data, sizeof (data));
                success = 1;

            }
                break;

            case SCRIPT_CMD_SPI_VERIFY:
            {
                uint8_t data[4];
                uint8_t i;
                uint8_t verifybyte;

                for (i = 0; i < 4; i++)
                    data[i] = flash_readbyte(scriptdata_p++);

                verifybyte = flash_readbyte(scriptdata_p++);

                isp_transmit(data, sizeof (data));

                if (data[3] == verifybyte) success = 1;
                else success = 0;
            }
                break;
            case SCRIPT_CMD_FLASH:
            case SCRIPT_CMD_EEPROM:
            {

                uint32_t address = (uint32_t) flash_readbyte(scriptdata_p++) << 24;
                address |= (uint32_t) flash_readbyte(scriptdata_p++) << 16;
                address |= (uint32_t) flash_readbyte(scriptdata_p++) << 8;
                address |= (uint32_t) flash_readbyte(scriptdata_p++);

                uint32_t length = (uint32_t) flash_readbyte(scriptdata_p++) << 24;
                length |= (uint32_t) flash_readbyte(scriptdata_p++) << 16;
                length |= (uint32_t) flash_readbyte(scriptdata_p++) << 8;
                length |= (uint32_t) flash_readbyte(scriptdata_p++);

                uint16_t pagesize = (uint16_t) flash_readbyte(scriptdata_p++) << 8;
                pagesize |= (uint16_t) flash_readbyte(scriptdata_p++);

                if (cmd == SCRIPT_CMD_FLASH) {
                    isp_writeFlash(scriptdata_p, address, length, pagesize);
                    success = isp_verifyFlash(scriptdata_p, address, length);
                } else {
                    isp_writeEEPROM(scriptdata_p, address, length, pagesize);
                    success = isp_verifyEEPROM(scriptdata_p, address, length);
                }

                scriptdata_p += length;
            }
                break;

            case SCRIPT_CMD_DECCOUNTER:
            {
                uint16_t startvalue = (uint16_t) flash_readbyte(scriptdata_p++) << 8;
                startvalue |= (uint16_t) flash_readbyte(scriptdata_p++);
                counter_decrement(startvalue);
                success = 1;
            }
                break;

			case SCRIPT_CMD_NOPROGRAM:
				return 2;
				break;

			case SCRIPT_CMD_END:
				return 1;
				break;
			
			//testing-commands following
			case SCRIPT_CMD_CHECKIO:
			{
				uint8_t port = flash_readbyte(scriptdata_p++);
				uint8_t pinNo = flash_readbyte(scriptdata_p++);
				uint8_t reference_state = flash_readbyte(scriptdata_p++);
				
				success = testing_checkIO(port, pinNo, reference_state);
				
			}
				break;

			
			case SCRIPT_CMD_SETIO:
			{
				uint8_t port = flash_readbyte(scriptdata_p++);
				uint8_t pinNo = flash_readbyte(scriptdata_p++);
				uint8_t InputOutput = flash_readbyte(scriptdata_p++);
				uint8_t state = flash_readbyte(scriptdata_p++);
				testing_setIO(port,pinNo,InputOutput,state);
				
				success=1;
				
			}
				break;

			
			
        }
		
		
		
        if (!success) {
            isp_disconnect();
            return 0;
        }
    }
}

