#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// software SPI:
#define OLED_MOSI  12
#define OLED_CLK   13
#define OLED_DC    10
#define OLED_CS    9
#define OLED_RESET 11

char battery[5][4] = {
"\x9D\x9E\x9F", 
"\x9D\x9E\xA3", 
"\x9D\xA1\xA3", 
"\x9D\xA2\xA3", 
"\xA0\xA2\xA3"
};
char signal[7][3] = {
"\x96\x96", 
"\x97\x96", 
"\x98\x96", 
"\x99\x96", 
"\x99\x9A", 
"\x99\x9B", 
"\x99\x9C", 
};
char zvuk[4][4] = {
"   ", 
")  ", 
")) ", 
")))"
};
char zvonok[3][6] = {
"  \xB3  ", 
" \xAB\xB3\xBB ",
"<\xAB\xB3\xBB>"
};
unsigned char horizontal[4] = 
{ 0x8C, 0x8D, 0x8E, 0x8F };
unsigned char vertical[6] = 
{ 0x90, 0x91, 0x92, 0x93, 0x94, 0x95 };
unsigned char strelki[8] = 
{ 0x18, 0xBD, 0x1A, 0xBF, 0x19, 0xBE, 0x1B, 0xBC };

unsigned char i1,i2,c3;

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup(){                 
  display.begin(SSD1306_SWITCHCAPVCC);
  display.cp437(true);
}

void loop() {
  unsigned char i,
    x1=6, y1=0,
    x2=108, y2=0,
    x3=0, y3=16,
    x4=0, y4=32,
    x5=6, y5=48,
    x6=42, y6=48,
    x7=108, y7=48;

  display.clearDisplay(); 
  display.setTextColor(WHITE);
  display.setTextSize(1);

  display.setCursor(0,0);
  display.println("\xAD   \xAE");

  display.setCursor(0,48);
  display.println("\xB2    \xB4       \xAF \xAC");

  randomSeed(millis());

  for(i=0; i<80; i++) {
    display.setTextColor(BLACK);

    display.setCursor(x1, y1);
    display.print("\x8B\x8B");

    display.setCursor(x2, y2);
    display.print("\x8B\x8B\x8B");

    display.setCursor(x3 + (i%20)*6, y3);
    display.write(0x8B);

    display.setCursor(x5, y5);
    display.print("\x8B\x8B\x8B");

    display.setCursor(x6, y6);
    display.print("\x8B\x8B\x8B\x8B\x8B");

    display.setCursor(x7, y7);
    display.write(0x8B);

    display.setTextColor(WHITE);

    display.setCursor(x1, y1);
    display.print(signal[i%7]);

    display.setCursor(x2, y2);
    display.print(battery[i%5]);
    
    display.setCursor(x3 + (i%20)*6, y3);
    display.write(vertical[random(5)]);

    display.setCursor(x4 + char(i/4)*6, y4);
    display.write(horizontal[i%4]);

    display.setCursor(x5, y5);
    display.print(zvuk[i%4]);

    display.setCursor(x6, y6);
    display.print(zvonok[i%3]);

    display.setCursor(x7, y7);
    display.write(strelki[i%8]);

    display.display();
    delay(1000);
  }
}

