/*
 * Author: Emre Şahin
 * Some parts of the animation code found on the internet and these parts belongs to the original authors.
 */
#include <EEPROM.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define VOLTMETERPIN A6

#include <Adafruit_NeoPixel.h>
#include "hsv.h"
#define MAXHUE 256*6
#define NEOPIN 12
#define NEONUMPIXELS 16 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NEONUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);
uint32_t fire_color   = pixels.Color ( 80,  35,  00);
uint32_t off_color    = pixels.Color (  40,  17,  0);
unsigned int oled_refresh_interval = 250;
bool screenState = true;
bool ledOn = EEPROM.read(1);
byte ledMode = EEPROM.read(0);
byte brightness = EEPROM.read(2);
int ledPosition = 0;
int ledHue = 0;

#define rotaryButton 2
#define outputA 3
#define outputB 4
int8_t rotaryState1 = 0;
int8_t rotaryState2 = 0;
int8_t rotaryCounter = 0;

unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long currentMillis;


///
/// Fire simulator
///
class NeoFire
{
    Adafruit_NeoPixel &pixels;
  public:

    NeoFire(Adafruit_NeoPixel&);
    void Draw();
    void Clear();
    void AddColor(uint8_t position, uint32_t color);
    void SubstractColor(uint8_t position, uint32_t color);
    uint32_t Blend(uint32_t color1, uint32_t color2);
    uint32_t Substract(uint32_t color1, uint32_t color2);
};
///
/// Constructor
///
NeoFire::NeoFire(Adafruit_NeoPixel& n_strip)
  : pixels (n_strip)
{
}

///
/// Set all colors
///
void NeoFire::Draw()
{
  Clear();

  for (int i = 0; i < NEONUMPIXELS; i++)
  {
    AddColor(i, fire_color);
    int r = random(80);
    uint32_t diff_color = pixels.Color ( r, r / 2, r / 2);
    SubstractColor(i, diff_color);
  }

  pixels.show();
}

///
/// Set color of LED
///
void NeoFire::AddColor(uint8_t position, uint32_t color)
{
  uint32_t blended_color = Blend(pixels.getPixelColor(position), color);
  pixels.setPixelColor(position, blended_color);
}

///
/// Set color of LED
///
void NeoFire::SubstractColor(uint8_t position, uint32_t color)
{
  uint32_t blended_color = Substract(pixels.getPixelColor(position), color);
  pixels.setPixelColor(position, blended_color);
}

///
/// Color blending
///
uint32_t NeoFire::Blend(uint32_t color1, uint32_t color2)
{
  uint8_t r1, g1, b1;
  uint8_t r2, g2, b2;
  uint8_t r3, g3, b3;

  r1 = (uint8_t)(color1 >> 16),
  g1 = (uint8_t)(color1 >>  8),
  b1 = (uint8_t)(color1 >>  0);

  r2 = (uint8_t)(color2 >> 16),
  g2 = (uint8_t)(color2 >>  8),
  b2 = (uint8_t)(color2 >>  0);

  return pixels.Color(constrain(r1 + r2, 0, 255), constrain(g1 + g2, 0, 255), constrain(b1 + b2, 0, 255));
}

///
/// Color blending
///
uint32_t NeoFire::Substract(uint32_t color1, uint32_t color2)
{
  uint8_t r1, g1, b1;
  uint8_t r2, g2, b2;
  uint8_t r3, g3, b3;
  int16_t r, g, b;

  r1 = (uint8_t)(color1 >> 16),
  g1 = (uint8_t)(color1 >>  8),
  b1 = (uint8_t)(color1 >>  0);

  r2 = (uint8_t)(color2 >> 16),
  g2 = (uint8_t)(color2 >>  8),
  b2 = (uint8_t)(color2 >>  0);

  r = (int16_t)r1 - (int16_t)r2;
  g = (int16_t)g1 - (int16_t)g2;
  b = (int16_t)b1 - (int16_t)b2;
  if (r < 0) r = 0;
  if (g < 0) g = 0;
  if (b < 0) b = 0;

  return pixels.Color(r, g, b);
}

///
/// Every LED to black
///
void NeoFire::Clear()
{
  for (uint16_t i = 0; i < pixels.numPixels (); i++)
    pixels.setPixelColor(i, off_color);
}
NeoFire fire(pixels);






void led(bool clearPixels = false) {
  if (clearPixels) {
    for (byte i = 0; i < NEONUMPIXELS; i++)
      pixels.setPixelColor(i, 0);
    pixels.show();
  } else {
    if (!ledOn)
      led(true);
    else {
      switch (ledMode) {
        case 0:
          for (int i = 0; i < NEONUMPIXELS; i++)
            pixels.setPixelColor((i + ledPosition) % NEONUMPIXELS, getPixelColorHsv(i, ledHue, 255, pixels.gamma8(i * (255 / NEONUMPIXELS))));
          ledPosition++;
          ledPosition %= NEONUMPIXELS;
          ledHue += 2;
          ledHue %= MAXHUE;
          break;
        case 1:
          if (currentMillis - previousMillis3 >= random(50, 150)) {
            previousMillis3 = currentMillis;
            fire.Draw();
          }
          break;
        case 2:
          for (byte i = 0; i < NEONUMPIXELS; i++)
            pixels.setPixelColor(i, pixels.Color(155, 75, 100));
          break;
        case 3:
          for (byte i = 0; i < NEONUMPIXELS; i++)
            pixels.setPixelColor(i, pixels.Color(45, 75, 75));
          break;
        case 4:
          for (byte i = 0; i < NEONUMPIXELS; i++)
            pixels.setPixelColor(i, pixels.Color(45, 150, 45));
          break;
        case 5:
          for (byte i = 0; i < NEONUMPIXELS; i++)
            pixels.setPixelColor(i, pixels.Color(55, 55, 55));
          break;
        default:
          ledMode = 0;
          break;
      }
      pixels.show();
    }

  }
}

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextWrap(false);         //Text overflow
  pixels.begin();
  pixels.clear();
  pixels.setBrightness(brightness);
  pinMode(outputA, INPUT_PULLUP);
  pinMode(outputB, INPUT);
  rotaryState2 = digitalRead(outputA);
  pinMode(2, INPUT);
  mainScreen();
  led(true);
}

void loop() {
  currentMillis = millis();
  rotaryEncoder();

  if (currentMillis - previousMillis >= oled_refresh_interval) {
    previousMillis = currentMillis;
    mainScreen();
  }
  if (currentMillis - previousMillis2 >= 100) {
    previousMillis2 = currentMillis;
    led();
  }
}

void rotaryEncoder() {
  rotaryState1 = digitalRead(outputA);
  if (rotaryState1 != rotaryState2)
    if (digitalRead(outputB) != rotaryState1)
      rotaryCounter++;
    else
      rotaryCounter--;
  rotaryState2 = rotaryState1;
}

void mainScreen() {
  bool button = digitalRead(2);
  if (!screenState) {
    if (button == HIGH) {
      screenState = true;
    }
  } else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(F("Battery: "));
    for (int8_t i = 0; i < analogRead(VOLTMETERPIN) / 1023.0 * 5; i++)
      display.print(F("I"));
    display.drawLine(0, 10, display.width() - 1, 10, SSD1306_WHITE);
    display.println();
    display.println();
    display.println(F("Select Option:"));
    display.println();
    display.println(F(" 1)ON    2)Brightness"));
    display.println();
    display.println(F(" 3)Mode  4)Close"));
    display.drawLine(0, display.height() - 1, display.width() - 1, display.height() - 1, SSD1306_WHITE);
    if (rotaryCounter >= 0) {//1
      rotaryCounter = 0;
      display.drawRect(0, 28, 45, 15, SSD1306_WHITE);//x,y,w,h
      if (button == HIGH && ledOn == false) {
        led();
        ledOn = true;
        EEPROM.write(1, ledOn);      //address 1: ledOn
      } else if (button == HIGH && ledOn == true) {
        led(true);
        ledOn = false;
        EEPROM.write(1, ledOn);      //address 1: ledOn
      }
    } else if (rotaryCounter >= -6 && rotaryCounter < 0) {//2
      display.drawRect(45, 28, 83, 15, SSD1306_WHITE);//x,y,w,h
      if (button == HIGH) {
        if (brightness > 220)
          brightness = 25;
        else {
          brightness += 25;
          pixels.setBrightness(brightness);
        }
        EEPROM.write(2, brightness); //address 2: brightness
      }
    } else if (rotaryCounter >= -12 && rotaryCounter < -6) {//3
      display.drawRect(0, 43, 45, 15, SSD1306_WHITE);//x,y,w,h
      if (button == HIGH) {
        if (ledMode > 5)
          ledMode = 0;
        else
          ledMode++;
        EEPROM.write(0, ledMode);    //address 0: ledMode
      }
    } else {//4
      rotaryCounter = -14;
      display.drawRect(45, 43, 83, 15, SSD1306_WHITE);//x,y,w,h
      if (button == HIGH) {
        screenState = false;
        display.clearDisplay();
        rotaryCounter = 0;
      }
    }
    display.display();
  }
}
