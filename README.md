# Nightscout-Wemos-OLED 
Nightscout blood sugar display on a Wemos with an oled display 
![Wemos with Integrated OLED](https://github.com/Hypertoken/Nightscout-Display/blob/main/Nightscout-Oled.jpg) 

All credit and thanks goes to the original project by robjohnc at https://github.com/robjohnc/nightscout_wemos_epaper
and c9679 at https://github.com/c9679/Nightscout-Wemos-OLED

This is a modified version of those. 

You will require the following:- 

Hardware:  
Wemos D1 Mini  
Blue / Yellow I2C OLED Display 

3D Printed Case (Credit to Qrome) - https://www.thingiverse.com/thing:2884823

Libraries:  
https://github.com/PaulStoffregen/Time  
https://github.com/bblanchon/ArduinoJson - Version 5  
https://github.com/ThingPulse/esp8266-oled-ssd1306  

Board Manager:  
ESP8266 - http://arduino.esp8266.com/stable/package_esp8266com_index.json  

Anyway hope someone finds this useful

1. Setup Arduino
2. Download libraries and drivers
3. Change Wifi details 
4. Add your Nightscout URL and SHA fingerprint
5. Change Timezone if needed
6. If needed change the screen pins e.g "SSD1306Wire  display(0x3c, D2, D5);"
7. Change sleeptime for the number of minutes between checks of the site default is every 1 minutes
8. Upload the sketch I have the board set to "LOLIN(WEMOS) D1 R2 & mini"  

Wiring:  
VCC = 3v3  
GND = GND  
SDA = D2  
SCL = D5  

