# ISPnubMod

Firmware for stand-alone AVR programmer module.

ISPnub is a stand-alone AVR programming module. The programming instructions
are defined within scripts. These scripts are converted with an additional
tool (ISPnubCreator) into a binary format and are stored in the flash of
ISPnub's ATmega1284P. This firmware interprets the binary programming
instructions and executes it on the connected target controller.
 
The firmware hex file is packed into the JAR file of ISPnubCreator which
merges the firmware hex data with programming instructions from scripts.

## Hardware / Boards

There are two dedicated boards to build the ISPnub. The original board is very small, further information you can get at the inventor of the <a href="http://www.fischl.de/ispnub/">original ISPnub</a>. The modded board <a href="https://oshpark.com/shared_projects/0ZfQJilI">can be ordered e.g. from OSHpark</a> and has some extra features on it:
  * Supported AVRs: ATmega1284, ATmega644, ATmega328@16MHz (Arduino Uno), ATmega32/16 (still untested)
  * 2 Power-Connectors
  * Buzzer
  * Connector for ISP 6-Header, Clamp and DIP-Socket
  * Internal vias for connecting a custom ISP-Header
  * Yellow LED for active programming state so the red one is used exclusively to display errors
  * Possibility to add a power switch (or just use a jumper)
  * 2 PINs breaked out for additional auxiliaries
Eagle files like schematics are to be found in ![board-subdir](/board)
![modded board](https://cdn.rawgit.com/mgroel/ISPnub/master/board/ispnub_mod_pics.svg)

If you would like to test the system on Arduino Uno first here is a connection diagram. Please be aware that the ISPnub-software only works if flashing the Arduino via ISP directly omitting the bootloader.
![try on arduino](https://cdn.rawgit.com/mgroel/ISPnub/master/arduino-testcircuit/arduino_isp_ispnub_Steckplatine.svg)


## Set up the Software / Usage

Detailed explanation about function and usage of the ISPnub are to be found at <a href="http://www.fischl.de/ispnub/">fischl.de</a>

In short:

### 1. Write a programming script (test.ispnub):
```
CONNECT 2000000                           ; connect with SCK = 2 MHz 
SPIVERIFY 0x30, 0x00, 0x00, 0x00, 0x1E    ; check signature byte 0x00 (0x1E = manufactured by Atmel)
SPIVERIFY 0x30, 0x00, 0x01, 0x00, 0x93    ; check signature byte 0x01 (0x93 = 8KB Flash memory)
SPIVERIFY 0x30, 0x00, 0x02, 0x00, 0x07    ; check signature byte 0x02 (0x07 = ATmega8 device)
SPIWRITE  0xAC, 0x80, 0x00, 0x00          ; chip erase
WAIT 2                                    ; wait 20 ms
FLASH ../test/main.hex, 0, 64             ; flash given hex file starting at flash address 0 with pagesize 64
DISCONNECT                                ; disconnect SPI
DECCOUNTER 10                             ; allow 10 programming cycles
END
```
### 2. Build ISPnub firmware:
Take the java tool ![ISPnubModCreator](https://github.com/mgroel/ISPnubCreator/releases) to build the firmware to be flashed to the ISPnub/Arduino.
Build the firmware like this for Arduino:
```
java -jar ISPnubCreator.jar test.ispnub atmega328_16MHz ispnub.hex
```

To use the modded firmware in compatibility-mode on the original ISPnub board execute:
```
java -jar ISPnubCreator.jar test.ispnub atmega1284_compat ispnub.hex
```

Or choose one of the other supported AVR in combination with the modded board:
```
java -jar ISPnubCreator.jar test.ispnub [atmega1284|atmega1284_compat|atmega328_16MHz|atmega16|atmega32|atmega644] ispnub.hex
```

### 3. Flash the ISPnub/Arduino:
Now the generated "ispnub.hex" can be flashed with your favorite programmer into the arduino. Take care for setting the fuses right. A working suggestion is given on the commandline by the java tool after building the firmware in step 2.

### 4. Flash targets by hitting the switch on the ISPnub
