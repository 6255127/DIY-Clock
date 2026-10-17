// tft display firmware - esp32 + st7789
// screen res is weird (284x76 not the normal 240x320) so theres some offset stuff below

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// pins - change these to match ur wiring!!
#define TFT_SCLK 0  // SCL on screen
#define TFT_MOSI 1  // SDA on screen
#define TFT_RST  2
#define TFT_DC   3
#define TFT_CS   4
#define TFT_BL   5

// adafruit lib doesnt expose setOffsets normally so we gotta subclass it
class MyST7789 : public Adafruit_ST7789 {
  public:
    MyST7789(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst)
      : Adafruit_ST7789(cs, dc, mosi, sclk, rst) {}

    void setOffsets(uint8_t col, uint8_t row) {
      _colstart = _colstart2 = col;
      _rowstart = _rowstart2 = row;
    }
};

MyST7789 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// just some stuff to keep track of so loop() isnt a mess
unsigned long lastUpdate = 0;
int counter = 0;

void setup() {
  Serial.begin(115200); // so we can talk to it over usb

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, LOW); // backlight is active low on this screen, LOW = on, annoying ik

  tft.init(76, 284);        // panel size, portrait
  tft.setOffsets(82, 18);   // fixes the weird resolution offset
  tft.invertDisplay(false); // screen colors come out inverted otherwise
  tft.setRotation(1);       // landscape - use 3 if its upside down for u

  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
  tft.setCursor(0, 0);
  tft.print("booting...");

  delay(500); // just so u can actually see the boot text lol

  Serial.println("tft ready");
}

void loop() {
  // basic example - counts up every second and redraws it
  // swap this out for whatever ur actually displaying (clock, menu, etc)

  if (millis() - lastUpdate > 1000) {
    lastUpdate = millis();
    counter++;

    tft.fillScreen(ST77XX_BLACK); // clear before redrawing or text overlaps
    tft.setCursor(0, 0);
    tft.setTextSize(4);
    tft.print(counter);
  }
}
