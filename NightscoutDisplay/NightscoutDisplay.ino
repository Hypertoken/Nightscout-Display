#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>
#include <SSD1306Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h> // have to use version 5.
#include <TimeLib.h>
#include <Adafruit_NeoPixel.h>


// Credentials 
#include "credentials.h" 

// timezone
const int timezone = -8; // -8 for PST
const int timezoneOffset = timezone * SECS_PER_MIN;

//OLED Screen Pins
SSD1306Wire display(0x3c, D2, D5);


//LED Count/Pins
Adafruit_NeoPixel pixels(1, D6);

//Miutes to sleep for
const int sleepTimeS = 1;

//You can hard code these variables here, if so, you should remove the line above to include the credentials.h. 
//However, for security you should use the credentials.h file
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;
const char* host = HOST_SITE;

const int httpsPort = 443;
const size_t bufferSize = 3*JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(13) + 280;
bool firstrun = true;
const char* directarr = "";

const unsigned char ArrowUp [] PROGMEM = {
  0x80, 0x00, 0xc0, 0x01, 0xe0, 0x03, 0xf0, 0x07, 0xf8, 0x0f, 0xfc, 0x1f, 0xde, 0x3d, 0xcf, 0x79, 0xc7, 0x71, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01,
};
const unsigned char ArrowDown [] PROGMEM = {
  0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc7, 0x71, 0xcf, 0x79, 0xde, 0x3d, 0xfc, 0x1f, 0xf8, 0x0f, 0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x01, 0x80, 0x00,
};
const unsigned char ArrowUpS [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0xf8, 0x3f, 0xf8, 0x3f, 0xf8, 0x3f, 0x00, 0x3f, 0x80, 0x3f, 0xc0, 0x3f, 0xe0, 0x3f, 0xf0, 0x39, 0xf8, 0x38, 0x7c, 0x38, 0x3c, 0x38, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char ArrowDownS [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x3c, 0x38, 0x7c, 0x38, 0xf8, 0x38, 0xf0, 0x39, 0xe0, 0x3f, 0xc0, 0x3f, 0x80, 0x3f, 0x00, 0x3f, 0xf8, 0x3f, 0xf8, 0x3f, 0xf8, 0x3f, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char ArrowSide [] PROGMEM = {
  0x80, 0x01, 0x80, 0x03, 0x80, 0x07, 0x00, 0x0f, 0x00, 0x1e, 0x00, 0x3c, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x3c, 0x00, 0x1e, 0x00, 0x0f, 0x80, 0x07, 0x80, 0x03, 0x80, 0x01,
};
const unsigned char ArrowUpD [] PROGMEM = {
  0x08, 0x10, 0x1c, 0x38, 0x2a, 0x54, 0x49, 0x92, 0x88, 0x11, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10,
};
const unsigned char ArrowDownD [] PROGMEM = {
  0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x88, 0x11, 0x49, 0x92, 0x2a, 0x54, 0x1c, 0x38, 0x08, 0x10,
};
const char* bgs0_sgv;

void adjustTimezone(time_t& timestamp) {
    timestamp += timezoneOffset * SECS_PER_MIN;
    if (isDST(timestamp)) {
        // Adjust for daylight saving time if needed
        timestamp += SECS_PER_HOUR;
    }
}

void showdata(time_t datetimenow, String BG, int age, String iob, String bgs0_direction, int delta)
{
   display.clear();

   // Convert 24-hour time format to 12-hour format
   int hour24 = hour(datetimenow);
   int hour12 = (hour24 % 12 == 0) ? 12 : hour24 % 12;
   String ampm = (hour24 >= 12) ? "PM" : "AM";

   String timeNow = String(hour12) + ":";
   if (minute(datetimenow) < 10){timeNow = timeNow + "0";} 
   timeNow = timeNow + String(minute(datetimenow)) + " " + ampm;

   display.setFont(ArialMT_Plain_10);
   display.setTextAlignment(TEXT_ALIGN_LEFT);
   display.drawString(2, 3, String(timeNow));
  
   display.setTextAlignment(TEXT_ALIGN_RIGHT);
   display.drawString(126, 3, String(age) + " min ago");
   display.setFont(ArialMT_Plain_24);
   display.setTextAlignment(TEXT_ALIGN_CENTER);
   display.drawString(45, 19, BG);
   display.drawString(45 + 1, 19, BG);
   if (String(bgs0_direction) == "Flat") {
      directarr = "→";
       display.drawXbm(101, 26, 16, 16,  ArrowSide);
    }  else if (String(bgs0_direction) == "FortyFiveUp") {
       directarr = "↗";
        display.drawXbm(101, 26, 16, 16,  ArrowUpS);
    }  else if (String(bgs0_direction) == "FortyFiveDown") {
       directarr = "↘";
        display.drawXbm(101, 26, 16, 16,  ArrowDownS);
    } else if (String(bgs0_direction) == "SingleUp") {
       directarr = "↑";
       display.drawXbm(101, 26, 16, 16,  ArrowUp);
    } else if (String(bgs0_direction) == "SingleDown") {
        directarr = "↓";
        display.drawXbm(101, 26, 16, 16, ArrowDown);
    } else if (String(bgs0_direction) == "DoubleUp") {
        directarr = "↑↑";
        display.drawXbm(101, 26, 16, 16, ArrowUpD);
    } else if (String(bgs0_direction) == "DoubleDown") {
        directarr = "↓↓";
        display.drawXbm(101, 26, 16, 16, ArrowDownD);
    }

   display.setFont(ArialMT_Plain_10);
   display.setTextAlignment(TEXT_ALIGN_LEFT);
   display.drawString(2, 50, "IOB: " + String(iob));
   display.setTextAlignment(TEXT_ALIGN_RIGHT);
   String bgdelta = "";
   if (delta < 0) {
    bgdelta = String(delta);
   } else if (delta > 0) {
    bgdelta = "+" + String(delta);
   } else {
    bgdelta = "±" + String(delta);
   }
   display.drawString(126, 50, bgdelta + " mg/dl ");
   display.drawRect(0, 0, 128, 64);
   display.drawRect(0, 17, 128, 33);
   display.drawVerticalLine(90, 20, 27);
   display.display();

}

bool isDST(time_t timestamp) {
    // DST starts in March and ends in November
    if (month(timestamp) < 3 || month(timestamp) > 11) {
        return false;
    }
    // DST is in effect from the second Sunday in March
    // to the first Sunday in November

    // Calculate the second Sunday in March
    int secondSunInMarch = 14 - ((5 * year(timestamp) / 4 + 1) % 7);

    // Calculate the first Sunday in November
    int firstSunInNovember = 7 - ((5 * year(timestamp) / 4 + 1) % 7);

    if (month(timestamp) == 3) {
        // Check if the date is on or after the second Sunday in March
        if (day(timestamp) > secondSunInMarch) {
            return true;
        } else if (day(timestamp) == secondSunInMarch && hour(timestamp) >= 2) {
            // Adjust for the start time of DST (typically 2 am local time)
            return true;
        }
    }

    if (month(timestamp) == 11) {
        // Check if the date is before the first Sunday in November
        if (day(timestamp) < firstSunInNovember) {
            return true;
        } else if (day(timestamp) == firstSunInNovember && hour(timestamp) < 2) {
            // Adjust for the end time of DST (typically 2 am local time)
            return true;
        }
    }
    // Months other than March and November
    return false;
}

void setpixel(String BGS) {
  int BG = BGS.toInt();
  if (BG > 200) {
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  } else if (BG < 70) {
    unsigned long currentTime = millis();
    int patternInterval = 50; // Adjust the interval to change the speed of the pattern
    int patternStep = currentTime / patternInterval % 8;

    if (patternStep == 0 || patternStep == 2) {
      pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // Red
    } else if (patternStep == 4 || patternStep == 6) {
      pixels.setPixelColor(0, pixels.Color(0, 0, 255)); // Blue
    } else {
      pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Off
    }
  } else if (BG < 90) {
    pixels.setPixelColor(0, pixels.Color(255, 128, 0));
  } else {
    pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  }
  pixels.show(); 
}

void getreadings() {
  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);

  client.setInsecure();

  if (firstrun) { 
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 26, "Connecting to NightScout");
    display.display();
    firstrun = false;
  }

  while (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    delay(30000);
  }

  String url = "/pebble";
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  //Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      //Serial.println("headers received");
      break;
    }
  }
  String line2 = client.readStringUntil('\n');
  Serial.println("==========");
  //read the JSON line into "line2"
  line2 = client.readStringUntil('\n');
  //parse the JSON into variables

  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.parseObject(line2);

  String status0_now = root["status"][0]["now"]; 
  status0_now = status0_now.substring(0, status0_now.length() - 3);
  time_t status0_now1 = status0_now.toInt();
  JsonObject& bgs0 = root["bgs"][0];

  bgs0_sgv = bgs0["sgv"]; 
  const char* bgs0_direction = bgs0["direction"]; 
  String bgs0_datetime = bgs0["datetime"]; 
  bgs0_datetime = bgs0_datetime.substring(0, bgs0_datetime.length() - 3);
  time_t bgs0_datetime2 = bgs0_datetime.toInt();
  //Adjust for Timezone and check for Daylight savings time and adjust display time accordingly
  adjustTimezone(status0_now1);
  adjustTimezone(bgs0_datetime2);
  
  int bgs0_bgdelta = bgs0["bgdelta"]; 
  const char* bgs0_iob = bgs0["iob"]; 
  
  Serial.print("Time Now: ");
  String timeNow = String(hour(status0_now1)) + ":";
  if (minute(status0_now1) < 10){timeNow = timeNow + "0";} 
  timeNow = timeNow + String(minute(status0_now1));
  Serial.println(timeNow);
  //print current time
  Serial.print("As at: ");
  String bgtime = String(hour(bgs0_datetime2)) + ":";
  if (minute(bgs0_datetime2) < 10){bgtime = bgtime + "0";} 
  bgtime = bgtime + String(minute(bgs0_datetime2));
  Serial.println(bgtime);
  int dataAge = (status0_now1 - bgs0_datetime2) / 60;
  Serial.print("Data Age: ");
  Serial.print(dataAge);
  Serial.println("m");
  Serial.print("Blood Glucose is:  ");
  Serial.println(String(bgs0_sgv) + " ");
  Serial.print("BG Delta is:  ");
  Serial.println(String(bgs0_bgdelta) + " ");
  //call the "showdata" function to show the data on the LCD screen
  showdata(status0_now1,bgs0_sgv,dataAge,bgs0_iob,bgs0_direction,bgs0_bgdelta);
  Serial.print("Direction:  ");
  Serial.println(directarr); 
  Serial.print("Insulin Onboard is:  ");
  Serial.println(String(bgs0_iob) + " ");
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);
  Serial.println();
  display.init();
  display.clear();
  pixels.begin(); 
  pixels.clear(); 
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 16, "Connecting to WIFI");
  display.drawString(64, 30, ssid);
  display.display();
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    pixels.setPixelColor(0, pixels.Color(0, 0, 255));
    pixels.show(); 
    delay(250);
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show(); 
    delay(250);
  }
  Serial.println(".");
  Serial.println("Connected to WIFI");
  Serial.println(WiFi.localIP());
}
void loop() 
{
  static unsigned long lastReadingsTime = 0;
  unsigned long currentTime = millis();

  // Run setpixel every loop iteration
  setpixel(bgs0_sgv);

  // Run getreadings every sleepTimeS minutes
  if (lastReadingsTime == 0 || currentTime - lastReadingsTime >= sleepTimeS * SECS_PER_MIN * 1000) {
    lastReadingsTime = currentTime;
    getreadings();
  }
}
