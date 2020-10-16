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

unsigned char i1,i2,c3;

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup(){                 
  display.begin(SSD1306_SWITCHCAPVCC);
  display.cp437(true);
}

void loop() {
  TEST_display_1();
  delay(10000);
  TEST_display_2();
  delay(10000);
  TEST_display_3();
  delay(8000);
  TEST_display_4();
  delay(4000);
}

void TEST_display_1()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  
  for(i1=0; i1<8; i1++) {
    for(i2=0; i2<16; i2++) {
      c3=i1*16+i2;
      if(c3 == 0x0A || c3 == 0x0D) display.print(" ");
      else display.write(c3);
    }
    display.println("");
  }
  display.display();
}

void TEST_display_2()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  
  for(i1=8; i1<16; i1++) {
    for(i2=0; i2<16; i2++)
      display.write(i1*16+i2);
    display.println("");
  }
  display.display();
}

void TEST_display_3()
{
  display.clearDisplay(); 
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);

  display.println(utf8rus("АБВГДЕЖЗИЙКЛМНОП"));
  display.println(utf8rus("РСТУФХЦЧШЩЪЫЬЭЮЯ"));
  display.println(utf8rus("абвгдежзийклмноп"));
  display.println(utf8rus("рстуфхцчшщъыьэюя"));
  display.println(utf8rus("Ёё123ABCabc!@#\xBC\xBD"));
  display.println(utf8rus("10\x83 10\x8A\x82 10\x81\x80 2\x85"));

  display.display();  
}

void TEST_display_4()
{
  display.clearDisplay(); 
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  display.setTextSize(1);
  display.println(utf8rus("Размер шрифта 1"));

  display.setTextSize(2);
  display.println(utf8rus("Размер 2"));

  display.setTextSize(3);
  display.println(utf8rus("Разм 3"));

  display.display();  
}

