# **LCD_Nokia_105_Drivers**
this given lcd i found from a old nokia 105 mobile phone. indeed, i enjoyed the journey and proccess to make it work like this.

9 bit spi data transmission is not easy by using hardware spi. because generally spi are in 8 bit,18 bit and 32 bit. but i need 9 bit spi clock. which why currently stick on bit bang register manipulation to simulate as api transmission.

Working Principle of Display on [Youtube Video][Linkyoutube]. just to share my experience.
- [Linkyoutube]: https://youtu.be/kFrF_M9M1sM 

## **SPFD54124B** Driver IC

![plot](./Document/LCD_PInout.png)

# **Support: Arduino Uno, ESP8266, ESP32_IDF And ESP32 WROOM32**
## Goals
-  [ ] sd card support.
## Accompolised
-  [x] Adafruit_gfx library: Spritegraphics,graphs,circle, rectangle,and triangle only on esp8266 and esp32.
-  [x] fast image processing.
-  [x] Header files contain comments.
-  [x] PCB design with - ESP12S,ESP32,and Breadboard Breakout.
-  [x] Compatibility : esp32, esp32_idf,AVR-AtMega328p and esp8266.
-  [x] Hardware SPI : esp32_idf. 

## Current Status
-  [ ] stm32.