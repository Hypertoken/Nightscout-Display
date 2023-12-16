# Nightscout-Wemos-OLED

Nightscout blood sugar display on a Wemos with an OLED display.

![Wemos with Integrated OLED](https://github.com/Hypertoken/Nightscout-Display/blob/main/Nightscout-Oled.jpg)

## Credits

All credit and thanks go to the original project contributors:
- [robjohnc](https://github.com/robjohnc/nightscout_wemos_epaper)
- [c9679](https://github.com/c9679/Nightscout-Wemos-OLED)

This is a modified version of those projects.

## Requirements

### Hardware:
- Wemos D1 Mini
- Blue/Yellow I2C OLED Display

### 3D Printed Case
Credit to Qrome - [Thingiverse link](https://www.thingiverse.com/thing:2884823)

### Libraries:
- [Time](https://github.com/PaulStoffregen/Time)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson) (Version 5)
- [esp8266-oled-ssd1306](https://github.com/ThingPulse/esp8266-oled-ssd1306)

### Board Manager:
- [ESP8266](http://arduino.esp8266.com/stable/package_esp8266com_index.json)

## Setup

1. **Setup Arduino:**
   - Install Arduino IDE.
   - Add ESP8266 board support.

2. **Download Libraries and Drivers:**
   - Install required libraries mentioned above.

3. **Configure Wifi Details:**
   - Update Wifi details in the sketch.

4. **Configure Nightscout Connection:**
   - Add your Nightscout URL and SHA fingerprint.

5. **Adjust Timezone:**
   - Change timezone if needed.

6. **Customize Screen Pins:**
   - Modify screen pins if necessary (e.g., "SSD1306Wire display(0x3c, D2, D5);").

7. **Set Sleep Time:**
   - Adjust the sleep time for the number of minutes between site checks (default is every 1 minute).

8. **Upload Sketch:**
   - Upload the sketch, ensuring the board is set to "LOLIN(WEMOS) D1 R2 & mini."

## Wiring

Connect the hardware as follows:

- VCC = 3v3
- GND = GND
- SDA = D2
- SCL = D5

## Contributing

If you'd like to contribute to this project, feel free to submit pull requests following the [contribution guidelines](CONTRIBUTING.md).

## License

This project is licensed under the [MIT License](LICENSE.md) - see the [LICENSE.md](LICENSE.md) file for details.

---

Anyway, hope someone finds this useful! Feel free to reach out if you have any questions or issues.
