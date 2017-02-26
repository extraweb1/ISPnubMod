ISPnubMod
=========

Firmware for stand-alone AVR programmer module.

ISPnub is a stand-alone AVR programming module. The programming instructions
are defined within scripts. These scripts are converted with an additional
tool (ISPnubCreator) into a binary format and are stored in the flash of
ISPnub's ATmega1284P. This firmware interprets the binary programming
instructions and executes it on the connected target controller.
 
The firmware hex file is packed into the JAR file of ISPnubCreator which
merges the firmware hex data with programming instructions from scripts.
 
![modded board](https://cdn.rawgit.com/mgroel/ISPnub/master/board/ispnub_mod_pics.svg)
<a href="https://oshpark.com/shared_projects/0ZfQJilI"><img src="https://oshpark.com/assets/badge-5b7ec47045b78aef6eb9d83b3bac6b1920de805e9a0c227658eac6e19a045b9c.png" alt="Order from OSH Park"></img></a>


Try on Arduino Uno
------------------
Test everything on Arduino first. Take the ![ISPnubModCreator](https://github.com/mgroel/ISPnubCreator/releases) to generate the firmware to be flashed to the Arduino. Please be aware, that this only works if the arduino is flashed via ISP directly, without the bootloader.
Build the firmware like this:
```
java -jar ISPnubCreator.jar test.ispnub atmega328_16MHz ispnub.hex
```
Now the generated "ispnub.hex" can be flashed with your favorite programmer into the arduino.
Further information you will find at the inventor of the ![original ISPnub](http://www.fischl.de/ispnub/)
![try on arduino](https://cdn.rawgit.com/mgroel/ISPnub/master/arduino-testcircuit/arduino_isp_ispnub_Steckplatine.svg)
