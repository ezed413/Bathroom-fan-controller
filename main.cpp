// sketch - Sec_bath_fan
// 3/8/2017    1.01
// Controls bathroom exhaust fan
// using DHT11 and arduino Nano
// 3/19/2017   1.02
// Added a bright white LED night light
// Also switched solid state relay from D13 to D12. D13 drives an on board LED
// Was getting a blip of the fan when power was applied by wall switch

// added on 3/26/17
// Changed the humidity setpoint from 65 to 75 5-15-2018
// #include <ESP8266WiFi.h>

// 11-22-2018 1.03
// changed the humidity setpoint from 75% to 80% relative humidity
#include <dht11.h>
//#include <Wire.h>
//#include <GOFi2cOLED.h>

//GOFi2cOLED GOFoled;

dht11 DHT11;

//#define DHT11PIN D0
#define DHT11PIN 17
#define HYSTERESIS 1


const char* ssid = "Google16";
const char* password = "wireless";
bool fanOn = false;
bool toggle_display = false;

float ConvTemp;
int temperature = 0;
int humidity = 0;
int chk;

//int relay = D1;  // fan relay

int ledPin = A0; // fan on indicator
int relay = 12;  // fan relay
int relay_control = 7;



//Celsius to Fahrenheit conversion
float Fahrenheit(float celsius)
{
  return 1.8 * celsius + 32;
}

void setup() {
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(relay_control, OUTPUT);
  Serial.begin(115200);
  //delay(10);

  // default slave address is 0x3D
  // GOFoled.init(0x3C);  //initialze  OLED display
  // GOFoled.display(); // show splashscreen
  // GOFoled.setTextColor(WHITE);
  // GOFoled.clearDisplay();
  // GOFoled.display();
}

void loop() {

  // fan switch just turned on


  // turn off transitor
  digitalWrite(relay_control, LOW);
  // turn fan on
  digitalWrite(ledPin, HIGH);
  digitalWrite(relay, HIGH);

  // GOFoled.setTextSize(2);
  // GOFoled.println("TechTron");
  // GOFoled.println("");
  // GOFoled.println("(C) 2018");
  // GOFoled.display();

  // wait 5 minutes
  for (int x = 0; x <= 300; x++) {
    delay(1000);
  }

  while (1) {

    // GOFoled.clearDisplay();
    // GOFoled.display();

    chk = DHT11.read(DHT11PIN);

    Serial.print("Read sensor: ");
    switch (chk)
    {
      case DHTLIB_OK:
        Serial.println("OK");
        break;
      case DHTLIB_ERROR_CHECKSUM:
        Serial.println("Checksum error");
        break;
      case DHTLIB_ERROR_TIMEOUT:
        Serial.println("Time out error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }


    // read sensor for temperature and relative humidity
    temperature = DHT11.temperature;
    humidity = DHT11.humidity;

    // convert celsius temperature to fahrenheit
    ConvTemp = Fahrenheit(temperature);
    //Changed the humidity setpoint from 75 to 80 11-22-2018
    // if humidity > 76 keep the exhaust fan on
    if (humidity >= (80 + HYSTERESIS)) {
      digitalWrite(ledPin, HIGH);
      digitalWrite(relay, HIGH);
      fanOn = true;
    }
    //Changed the humidity setpoint from 75 to 80 11-22-2018
    // if humidity drops below 74, than turn the exhaust fan off
    if (humidity <= (80 - HYSTERESIS)) {
      digitalWrite(ledPin, LOW);
      digitalWrite(relay, LOW);
      fanOn = false;
    }

    // display temp and humidity on the OLED display
    if (toggle_display) {
      // GOFoled.setTextSize(2);
      // GOFoled.setTextColor(WHITE);
      // GOFoled.setCursor(0, 0);
      //GOFoled.print("Temperature: ");
      //GOFoled.println(ConvTemp, 1);
      // GOFoled.println("Humidity: ");
      // GOFoled.setTextSize(3);
      // GOFoled.print(humidity, DEC);
      // GOFoled.println("%");
      // GOFoled.display();
      // toggle_display = !toggle_display;
    }
    else {
      // GOFoled.setTextSize(4);
      // GOFoled.setCursor(0, 0);
      // if (fanOn) {
      //   GOFoled.println("Fan");
      //   GOFoled.println("ON");
      // }
      // else {
      //   GOFoled.println("Fan");
      //   GOFoled.println("OFF");
      // }
      //
      // GOFoled.display();
      // toggle_display = !toggle_display;
    }

    delay(1000);
  }
}
