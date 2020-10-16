//display.cp437(true);
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_ST77xx.h>

//#include <c:\Users\denis\Documents\Arduino\bottle_test\iarduino_OLED.h>
#include "qrcode.h"
//iarduino_OLED myOLED(0x3C);
//extern uint8_t MediumFontRus[];
#include <SPI.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_PCD8544.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"


//Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 6, 7);
Adafruit_ST7735 display = Adafruit_ST7735(10, 9, 8);


/* Initialise COLOR SENSOR with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

//LiquidCrystal lcd(8, 7, 6, 5, 4, 3);

#define LEDpin 2      // питание светодиода
#define IRpin A1      // питание фототранзистора
#define IRsens A2     // сигнал фототранзистора
#define CSpower A0		// питание датчика цвета
#define S0 8
#define S1 9
#define S2 10
#define S3 11
#define CSout 12


//colors
#define GREEN    0x07E0
#define WHITE    0xFFFF
#define BLUE     0xF800 //0x001F
#define BROWN    0xA5AA //0xA3AE
#define BLACK    0x0000

//кнопка
#define button1 4
//концевик
#define switch1 3
//шредер
#define shred1 5
//заслонка
#define flap1 6
//носик
#define spout1 7
//дрежнаж
#define drain1 12

unsigned long previousMillis = 0;
const long interval = 2000; 

const String BinId = "1";


bool scanbottle = false;
bool freepipe = true;
bool allow_shred = false;
bool allow_flap = false;
bool getbreak = false;

int sens_signal, last_sens_signal, empty_sens_signal, bottle_counter = 0, test_time=30;
float red, green, blue;
uint8_t rc, gc, bc;

// The setup() function runs once each time the micro-controller starts

void setup()
{
//myOLED.begin();                                        // Инициируем работу с дисплеем.
//myOLED.clrScr(true);
//myOLED.setFont(MediumFontRus);
//myOLED.setCursor(0,15);
//myOLED.setCoding(TXT_UTF8); 
//myOLED.print( F("Нажмите"));
//myOLED.setCursor(0,35);
//myOLED.print("\n\r");
//myOLED.print( F("кнопку"));

//display.begin();
//rotation LCD
//display.setRotation(1);


 display.initR(INITR_GREENTAB);
 display.invertDisplay(0); 
 display.fillScreen(ST77XX_WHITE);
 display.setTextColor(0x0000);
 display.setTextSize(2);
 
 display.setCursor(0,0);

Serial.begin(9600);


pinMode(LEDpin, OUTPUT);
pinMode(IRpin, OUTPUT);
pinMode(CSpower, OUTPUT);
pinMode(IRsens, INPUT);
pinMode(S0, OUTPUT);
pinMode(S1, OUTPUT);
pinMode(S2, OUTPUT);
pinMode(S3, OUTPUT);
pinMode(CSout, INPUT);
pinMode(button1,INPUT_PULLUP);
pinMode(switch1,INPUT_PULLUP);
pinMode(shred1,OUTPUT);
pinMode(flap1,OUTPUT);
pinMode(spout1,OUTPUT);
pinMode(drain1,OUTPUT);

digitalWrite(S0,HIGH);
digitalWrite(S1,LOW);

digitalWrite(LEDpin, 1);
digitalWrite(IRpin, 1);
digitalWrite(CSpower, 1);

digitalWrite(shred1, 0);
digitalWrite(flap1, 0);
digitalWrite(spout1, 0);
digitalWrite(drain1, 0);


display.fillScreen(ST77XX_WHITE);
display.print(utf8rus("Натиснiть кнопку для початку"));
//display.fillTriangle(52, 80, 72, 80, 62, 100,GREEN);
while(1)
{
 if(digitalRead(button1) == LOW)
 {
  delay(500);
  break;
 }
}

if(allow_shred == true)
{
  digitalWrite(shred1, HIGH);
}
else
{
  digitalWrite(shred1, LOW);
}

if(allow_flap == true)
{
  digitalWrite(flap1, HIGH);
}
else
{
  digitalWrite(flap1, LOW);
}


//калибровка пустой трубы
display.fillScreen(ST77XX_WHITE);
display.setCursor(0,0);
display.print(utf8rus("Калiбрування давачiв. Зачекайте..."));
unsigned long currentMillis = millis();
while(millis() < (currentMillis + interval) )
{
  sens_signal=analogRead(IRsens);
	if(sens_signal > last_sens_signal ) last_sens_signal = sens_signal;
}
Serial.print("Empty pipe: ");
Serial.println(last_sens_signal);
empty_sens_signal = last_sens_signal;


//Калибровка датчика цвета
tcs.getRGB(&red, &green, &blue);
//White ballance

if(red < green && red < blue)
{
  rc = 0;
  bc = blue-red;
  gc = green-red;
  green = green - (green-red);
  blue = blue - (blue-red);
}
if(green < red && green < blue)
{
  rc = red-green;
  bc = blue-green;
  gc = 0;
  red = red - (red-green);
  blue = blue - (blue-green);
}
if(blue < red && blue < green)
{
  rc = red-blue;
  bc = 0;
  gc = green-blue;
  red = red - (red-blue);
  green = green - (green-blue);
}
//red = red-0;
//green = green-8;
//blue = blue - 1;
Serial.print("R:\t"); Serial.print(int(red)); 
Serial.print("\tG:\t"); Serial.print(int(green)); 
Serial.print("\tB:\t"); Serial.print(int(blue));
Serial.print("\n"); 
delay(500);
//==================================================================    test IRsens-1
/*display.print(utf8rus("Тест датчиків 30 секунд..."));
while(test_time>0){
  sens_signal = analogRead(IRsens);
  Serial.print( "IRsens: ");
  Serial.print(sens_signal);
  Serial.print( "\n");
  test_time--;
  delay(1000);
}*/
//==================================================================    end test IRsens-1

 display.fillScreen(ST77XX_WHITE);
 display.setCursor(0,0);
 display.print(utf8rus("Помiстiть пляшку та закрийте кришку"));
 display.fillTriangle(52, 100, 72, 100, 62, 80,ST77XX_GREEN); 
 display.drawTriangle(52, 100, 72, 100, 62, 80,BLACK);
// display.display();
}

// Add the main program code into the continuous loop() function
void loop()
{
unsigned long currentMillis = millis();

if(digitalRead(switch1) == LOW)
{
  delay(200);
  scanbottle = true;
  digitalWrite(shred1,HIGH);
  //Serial.print( "\n Closed.");
}
else
{
  delay(200);
  scanbottle = false;
  freepipe = true;
  digitalWrite(shred1,LOW);
  //Serial.print( "\n Open.");
}


if(scanbottle == true)
{
//Опрос ик датчика
while(millis() < (currentMillis + interval) )
  {
	  if(digitalRead(button1) == LOW)
    {
      getbreak = true;
      delay(50);
    }
 	  sens_signal = analogRead(IRsens);
    //Serial.print( "\n IRsens: ");
    //Serial.print(sens_signal);
	  if( sens_signal > last_sens_signal ) last_sens_signal = sens_signal;	
  }

// =================================================      Test IRsens-2
/*Serial.print( "\n difference: ");
Serial.print(last_sens_signal - empty_sens_signal);
Serial.print( "\n last_sens_signal: ");
Serial.print(last_sens_signal);
*/
// =================================================      End test IRsens-2
//Бутылка или нет
if( ((last_sens_signal - empty_sens_signal) >= 10) && ((last_sens_signal - empty_sens_signal) < 700 ))  //25-150
{
  if(freepipe)
  {
    bottle_counter++;
	  Serial.print( "\n Bottle detected! ");
	  Serial.print(bottle_counter);
	  Serial.print(" - ");
	  Serial.println(last_sens_signal);

    display.fillScreen(ST77XX_WHITE);
    display.setCursor(0,0);
    display.print(utf8rus("Пляшок: "));
    display.println(bottle_counter);
 
 

  //get color
  tcs.getRGB(&red, &green, &blue);
  red = red-rc;
  green = green-gc;
  blue = blue-bc;
  
  uint8_t color_counter=0;
  
  if(( abs((red - green)) < 6 && abs((red - blue)) < 6 && abs((blue - green)) < 6) || (abs(blue-green) <= 5))
  {
    Serial.print("Bottle is transparent"); Serial.print("\n");  
    display.println(utf8rus("Прозора пляшка"));
    color_counter = 3; 
    display.drawCircle(64, 80, 20, BLACK);
  }
  
  else
  {
  if(red > green && red > blue && (red - green) > 15) //(red > green && red > blue)
  {
    Serial.print("Bottle is brown"); Serial.print("\n");
    display.println(utf8rus("Коричнева пляшка"));
    color_counter = 1;
    display.fillCircle(64, 80, 20, BROWN);
    display.drawCircle(64, 80, 20, BLACK);
  }
  
  if(green > red && green > blue && (green-blue) > 15)
  {
    Serial.print("Bottle is green"); Serial.print("\n");
    display.println(utf8rus("Зелена пляшка"));
    color_counter = 2;
    display.fillCircle(64, 80, 20, GREEN);
    display.drawCircle(64, 80, 20, BLACK);
  }
  
  if(blue > red && blue > green && (blue-green) > 15)
  {
    Serial.print("Bottle is blue"); Serial.print("\n");  
    display.println(utf8rus("Блакитна пляшка"));
    color_counter = 4;
    display.fillCircle(64, 80, 20, BLUE);
    display.drawCircle(64, 80, 20, BLACK);
  }
  }

  Serial.print("R:\t"); Serial.print(int(red)); 
  Serial.print("\tG:\t"); Serial.print(int(green)); 
  Serial.print("\tB:\t"); Serial.print(int(blue));
  Serial.print("\n"); 
  freepipe = false;

delay(1000);

display.fillScreen(ST77XX_WHITE);
display.setCursor(0,0);
display.println(utf8rus("Утилiзацiя, зачекайте..."));

display.fillTriangle(52, 80, 72, 80, 62, 100,ST77XX_RED);
display.drawTriangle(52, 80, 72, 80, 62, 100,BLACK);

//Поворачиваем носик
for (int i=0;i<color_counter;i++)
{
  digitalWrite(spout1, HIGH);
  delay(500);
  digitalWrite(spout1, LOW);
  delay(500);
}

//открываем заслонку и ждем
digitalWrite(flap1, HIGH);
delay(5000);
digitalWrite(flap1, LOW);


display.fillScreen(ST77XX_WHITE);
display.setCursor(0,0);
display.println(utf8rus("Вiдкрийте кришку та помiстiть пляшку, або натиснiть кнопку для генерування коду"));


}


/*display.fillScreen(ST77XX_WHITE);
display.setCursor(0,0);
display.println(utf8rus("Не кидайте смiття"));
digitalWrite(drain1, HIGH);
delay(1000);
digitalWrite(drain1, LOW);
*/

}
else
{
  freepipe = true;
}
}

last_sens_signal = 0;

//генерация qr
if(digitalRead(button1) == LOW || getbreak == true)
//if(bottle_counter == 2 && scanbottle == true)
{
  digitalWrite(shred1, 0);
  digitalWrite(flap1, 0);
  digitalWrite(spout1, 0);
  digitalWrite(drain1, 0);
  scanbottle = false;
  display.fillScreen(ST77XX_WHITE);
  generateQR(bottle_counter);
  delay(20000);
  asm("JMP 0");
}
}


void generateQR(uint8_t count)
{
uint8_t px=20; 
uint8_t py=20;
  // Create the QR code
QRCode qrcode;
uint8_t qrcodeData[qrcode_getBufferSize(3)];

String qrstring = "{\"binid\":" + BinId + ",\"count\":" + String(count) +",\"time\":" + String(millis()) + "}";

qrcode_initText(&qrcode, qrcodeData, 3, ECC_LOW, qrstring.c_str());

    for (uint8_t y = 0; y < qrcode.size; y++) {

        // Left quiet zone
        //Serial.print("        ");
           for(uint8_t c=0; c<3; c++)
            {
        // Each horizontal module
        for (uint8_t x = 0; x < qrcode.size; x++) {
            //  Print each module (UTF-8 \u2588 is a solid block)
            //  myOLED.print(qrcode_getModule(&qrcode, x, y) ? "." : " ");
            //  myOLED.print("**");
     
            if(qrcode_getModule(&qrcode, x, y))
            {
            //  myOLED.drawPixel(px, py, 0);    px++;
            //  myOLED.drawPixel(px, py, 0);  px++;
            //  myOLED.drawPixel(px, py, 0);  px++;
            //  myOLED.drawPixel(px, py, 0);  px++;
            display.drawPixel(px,py, ST77XX_BLACK); px++;
            display.drawPixel(px,py, ST77XX_BLACK); px++;
            display.drawPixel(px,py, ST77XX_BLACK); px++;
            //  display.drawPixel(px,py, BLACK); px++;
            //  display.drawPixel(px,py, BLACK); px++;
            //  display.drawPixel(px,py, BLACK); px++;
            
            }
            else
            {
              px++;
              px++;
              px++;
              // px++;
              // px++;
              // px++;
              // px++;
            }
        }
 //  display.display();
   //     Serial.println((px/2)-10);
        py++;
   //     py++;
  //      py++;
        px=20;
            
    }
        //}
        //Serial.print("\n");
    }
   
}

String utf8rus(String source)
{
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };

  k = source.length(); i = 0;

  while (i < k) {
    n = source[i]; i++;

    if (n >= 0xBF){
      switch (n) {
        case 0xD0: {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x2F;
          break;
        }
        case 0xD1: {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB7; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x6F;
          break;
        }
      }
    }
    m[0] = n; target = target + String(m);
  }
return target;
}
