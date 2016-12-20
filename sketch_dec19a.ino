#include <SoftwareSerial.h>

#include "JQ6500.h"

#include <Adafruit_NeoPixel.h>
#include "U8glib.h"
#include <EEPROM.h>              //引入函数库

SoftwareSerial audioSerial(2,3);
JQ6500 AUDIO(&audioSerial);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, 2, NEO_GRB + NEO_KHZ800);
long a;
long a_random;
long b;
long score;
long bingo;
long hold;
long miss;
long time1;
long time2;
long top;
unsigned long timer2lastTime = millis();
unsigned long timer1lastTime = millis();
boolean joy_stick(int pin, int mode) {                        //定义摇杆输入
uint8_t joyStickA=-1,joyStickB=-2;
  while(joyStickA!=joyStickB){
    if (analogRead(pin) < 50) {
      joyStickA=0;
    }
    else if(analogRead(pin) < 374 && analogRead(pin) > 274){
      joyStickA=1;
    }
    else if(analogRead(pin) < 559 && analogRead(pin) > 459){
      joyStickA=2;
    }
    else if(analogRead(pin) < 751 && analogRead(pin) > 651){
      joyStickA=3;
    }
    else if(analogRead(pin) < 906 && analogRead(pin) > 806){
      joyStickA=4;
    }
    else if(analogRead(pin) > 973){
      joyStickA=5;
    }
    delay(20);
    if (analogRead(pin) < 50) {
      joyStickB=0;
    }
    else if(analogRead(pin) < 374 && analogRead(pin) > 274){
      joyStickB=1;
    }
    else if(analogRead(pin) < 559 && analogRead(pin) > 459){
      joyStickB=2;
    }
    else if(analogRead(pin) < 751 && analogRead(pin) > 651){
      joyStickB=3;
    }
    else if(analogRead(pin) < 906 && analogRead(pin) > 806){
      joyStickB=4;
    }
    else if(analogRead(pin) > 973){
      joyStickB=5;
    }
  }
  if(mode==joyStickB){
    return true;
  }
  else{
    return false;
  }
}

#define BuzzerPin8 8    //定义蜂鸣器端口
int tone_list[] = {262, 294, 330, 349, 392, 440, 494, 523, 587, 659, 698, 784, 880, 988, 1046, 1175, 1318, 1397, 1568, 1760, 1967};
int music_1[] = {12, 10, 12, 10, 12, 10, 9, 10, 12, 12, 12, 10, 13, 12, 10, 12, 10, 9, 8, 9, 10, 12, 10, 9, 8, 9, 10, 0};
float rhythm_1[] = {1, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 2, 0.5, 1, 0.5, 1, 1, 0.5, 0.5, 0.5, 0.5, 1, 1, 1, 1, 0.5, 0.5, 0.5, 0.5, 2};
int music_2[] = {8, 9, 10, 8, 8, 9, 10, 8, 10, 11, 12, 10, 11, 12, 0};
float rhythm_2[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2};
int music_3[] = {5, 8, 8, 10, 13, 10, 12, 12, 13, 12, 10, 11, 10, 9, 6, 9, 9, 11, 14, 14, 13, 12, 11, 11, 10, 6, 7, 8, 9, 0};
float rhythm_3[] = {0.5, 0.25, 0.5, 0.25, 0.5, 0.25, 1, 0.5, 0.25, 0.5, 0.25, 0.5, 0.25, 1, 0.5, 0.25, 0.5, 0.25, 0.5, 0.25, 0.5, 0.25, 1, 0.5, 0.25, 0.5, 1, 0.5, 3};
int music_4[] = {5,5,6,5,8,7,5,5,6,5,9,8,5,5,12,10,8,7,6,11,11,10,8,9,8,0};
float rhythm_4[] = {0.5,0.5,1,1,1,2,0.5,0.5,1,1,1,2,0.5,0.5,1,1,1,1,1,0.5,0.5,1,1,1,3};
int music_5[] = {12, 13, 12, 13, 12, 13, 12, 12, 15, 14, 13, 12, 13, 12, 12, 12, 10, 10, 12, 12, 10, 9, 11, 10, 9, 8, 9, 8, 0};
float rhythm_5[] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1};
int music_6[] = {8, 8, 10, 8, 8, 10, 22, 13, 13, 13, 12, 13, 12, 8, 10, 22, 15, 13, 13, 12, 13, 12, 8, 9, 22, 14, 14, 12, 10, 12, 0};
float rhythm_6[] = {1, 1, 2, 0.5, 1, 1, 1, 1, 1, 0.5, 0.5, 1, 0.5, 1, 1, 1, 0.5, 0.5, 0.5, 0.5, 2, 0.5, 1, 1, 1, 1, 0.5, 0.5, 1, 4};
int music_7[] = {6, 8, 9, 10, 12, 10, 8, 9, 6, 22, 8, 9, 10, 12, 12, 13, 9, 10, 22, 10, 12, 13, 12, 13, 15, 14, 13, 12, 13, 10, 8, 9, 10, 12, 8, 6, 8, 9, 10, 13, 12, 0};
float rhythm_7[] = {0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 1, 2, 0.5, 0.5, 0.5, 0.5, 1, 0.5, 1, 1, 2, 1, 0.5, 0.5, 2, 1, 0.5, 0.5, 0.25, 0.25, 0.5, 0.5, 1, 0.5, 0.5, 1, 0.5, 1, 1, 0.5, 0.5, 0.5, 0.5, 3};
int music_8[] = {10, 8, 9, 6, 10, 9, 8, 9, 6, 10, 8, 9, 9, 12, 10, 7, 8, 8, 7, 6, 7, 8, 9, 5, 13, 12, 10, 10, 9, 8, 9, 10, 9, 10, 9, 12, 12, 12, 12, 12, 12, 0};
float rhythm_8[] = {1, 1, 1, 1, 0.5, 0.5, 0.5, 0.5, 2, 1, 1, 1, 1, 0.5, 0.5, 1, 1, 0.5, 0.5, 1, 0.5, 0.5, 1, 1, 0.5, 0.5, 1, 1, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 1};
int music_9[] = {10,12,15,13,12,10,12,13,15,12,15,17,16,15,16,15,13,15,12,0};
float rhythm_9[] = {0.5,0.5,0.5,0.5,2,0.5,0.5,0.5,0.5,2,1,0.5,1,1,0.5,0.5,0.5,0.5,2};
int music_10[] = {10,10,10,8,5,5,22,10,10,10,8,10,22,12,12,10,8,5,5,5,6,7,8,10,9,0};
float rhythm_10[] = {0.5,0.5,0.5,0.5,1,0.5,0.5,0.5,0.5,0.5,0.5,1,1,0.5,0.5,0.5,0.5,0.5,0.5,1,0.5,0.5,0.5,0.5,1};

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
#define setFont_L u8g.setFont(u8g_font_fur20)
#define setFont_S u8g.setFont(u8g_font_fixed_v0r)
#define setFont_M u8g.setFont(u8g_font_9x18)

static unsigned char hai[] U8G_PROGMEM = {
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xC7,0xFF,0xFF,0xFF,0xFF,0xE0,0xFF,0xFF,0x07,0xFF,0xFF,0xFF,0x0F,0x80,0xFF,
0xFF,0x0F,0xFC,0xFF,0x1F,0x00,0x80,0xFF,0xFF,0x1F,0xF8,0x03,0x00,0x00,0xF8,0xFF,
0xFF,0x3F,0xF8,0x01,0x00,0xFE,0xFF,0xFF,0xFF,0x7F,0xF8,0x03,0x3C,0xFC,0xFF,0xFF,
0xFF,0xFF,0xF8,0xFF,0x3F,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0xFC,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x1F,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x81,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x83,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xC3,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC3,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xC1,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0xFF,0xFF,0xFF,
0xFF,0x1F,0xFE,0xFF,0xC0,0xFF,0xFF,0xFF,0xFF,0x0F,0xFC,0x7F,0xC0,0xFF,0xFF,0xFF,
0xFF,0x01,0xF8,0x3F,0xC4,0xFF,0xFF,0xFF,0x3F,0x00,0xFC,0x1F,0xC6,0x0F,0xFE,0xFF,
0xFF,0x08,0xFE,0x1F,0xC7,0x1F,0xF8,0xFF,0xFF,0x1F,0xFE,0x0F,0xC7,0x3F,0xE0,0xFF,
0xFF,0x1F,0xFF,0x87,0xC7,0x7F,0xC0,0xFF,0xFF,0x0F,0xFF,0xC3,0xC7,0xFF,0x80,0xFF,
0xFF,0x8F,0xFF,0xE1,0xC7,0xFF,0x81,0xFF,0xFF,0x8F,0xFF,0xF0,0xC7,0xFF,0x83,0xFF,
0xFF,0x8F,0x7F,0xF8,0xC7,0xFF,0x87,0xFF,0xFF,0x8F,0x3F,0xFE,0xC7,0xFF,0x8F,0xFF,
0xFF,0x8F,0x9F,0xFF,0xC7,0xFF,0xFF,0xFF,0xFF,0x1F,0xEF,0xFF,0xC7,0xFF,0xFF,0xFF,
0xFF,0x1F,0xFF,0xFF,0xC7,0xFF,0xFF,0xFF,0xFF,0x3F,0xFF,0xFF,0xC7,0xFF,0xFF,0xFF,
0xFF,0x3F,0xFF,0xFF,0xC3,0xFF,0xFF,0xFF,0xFF,0x3F,0xFF,0xFF,0xC3,0xFF,0xFF,0xFF,
0xFF,0x3F,0xFF,0xFF,0xC3,0xFF,0xFF,0xFF,0xFF,0x3F,0xFF,0xFF,0xC3,0xFF,0xFF,0xFF,
0xFF,0x03,0xFC,0xFF,0xC7,0xFF,0xFF,0xFF,0x0F,0x00,0xC0,0xFF,0xC7,0xFF,0xFF,0xFF,
0x07,0xF0,0x07,0xF8,0xEF,0xFF,0xFF,0xFF,0x0F,0xFE,0x3F,0x80,0xEF,0xFF,0xFF,0xFF,
0xBF,0xFF,0xFF,0x01,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0x00,0xFE,0xFF,0xFF,
0xFF,0xFF,0xFF,0x3F,0x00,0x00,0x00,0xC0,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x00,0xF0,
0xFF,0xFF,0xFF,0xFF,0x07,0x00,0x00,0xFC,0xFF,0xFF,0xFF,0xFF,0x3F,0x00,0x00,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0x01,0xC0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0xF8,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};                                                                              //汉字“还”字形码
static unsigned char xu[] U8G_PROGMEM = {
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0xF8,0xFF,0xFF,
0xFF,0xFF,0xFF,0x3F,0x00,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,
0xFF,0xFF,0x0F,0x00,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x00,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x8F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x8F,0xFF,0xFF,0xFC,0xFF,
0xFF,0xFF,0xFF,0x8F,0xFF,0x1F,0xF0,0xFF,0xFF,0xFF,0xFF,0x8F,0xFF,0x00,0xC0,0xFF,
0xFF,0xFF,0xFF,0x8F,0x03,0x00,0x80,0xFF,0xFF,0xFF,0xFF,0x0F,0x00,0xFE,0x03,0xFF,
0xFF,0xEF,0x7F,0x00,0xFC,0xFF,0x01,0xFE,0xFF,0x07,0x00,0x80,0xFF,0xFF,0x01,0xFF,
0xFF,0x07,0xC0,0x8F,0x0F,0xFE,0xF1,0xFF,0xFF,0xC7,0xFF,0x8F,0x0F,0xFC,0xFC,0xFF,
0xFF,0xC3,0xFF,0x8F,0x3F,0x7C,0xFE,0xFF,0xFF,0xE3,0xC7,0x8F,0xFF,0xFF,0xFF,0xFF,
0xFF,0xE1,0x03,0x8F,0xFF,0xFF,0xFF,0xFF,0xFF,0xE1,0x0F,0x8F,0xFF,0xFF,0xFF,0xFF,
0xFF,0xF0,0xFF,0x8F,0x0F,0xFE,0xFF,0xFF,0xFF,0xF0,0xFF,0x8F,0x0F,0xF8,0xFF,0xFF,
0x7F,0xF8,0xFF,0x9F,0x3F,0xF8,0xFF,0xFF,0xFF,0xFC,0xF3,0x9F,0xFF,0xFD,0xFF,0xFF,
0xFF,0xFF,0x81,0x9F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x83,0x9F,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0x9F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x01,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0x1F,0x00,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x00,0xE0,0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0x00,0xFE,0xFF,0xFF,0xFF,
0xFF,0xFF,0x01,0xE0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE1,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xF1,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF1,0xFF,0x87,0xFF,0xFF,
0xFF,0xFF,0xFF,0xF8,0x7F,0x00,0xFE,0xFF,0xFF,0xFF,0xFF,0xFC,0x01,0x00,0xFC,0xFF,
0xFF,0xFF,0x7F,0x00,0x00,0x1F,0xF8,0xFF,0xFF,0xCF,0x07,0x00,0xF8,0x1F,0xF8,0xFF,
0xFF,0x0F,0x00,0x7C,0xF8,0x1F,0xFC,0xFF,0xFF,0x1F,0x3E,0x7E,0xF8,0x1F,0xFC,0xFF,
0xFF,0x1F,0x3E,0x7E,0xFC,0x1F,0xFE,0xFF,0xFF,0x1F,0x3E,0x7E,0xFC,0x1F,0xFE,0xFF,
0xFF,0x1F,0x3E,0x7E,0xFC,0x1F,0xFE,0xFF,0xFF,0x1F,0x3E,0x7E,0xFC,0x1F,0xFE,0xFF,
0xFF,0x1F,0x3E,0x7E,0xFC,0x1F,0xFE,0xFF,0xFF,0x1F,0x3E,0x7E,0xFC,0x1F,0xFE,0xFF,
0xFF,0x3F,0x7E,0x7E,0xFC,0x0F,0xFF,0xFF,0xFF,0x3F,0x7E,0x7F,0xBE,0x0F,0xFF,0xFF,
0xFF,0x3F,0x7E,0x7F,0x7E,0x00,0xFF,0xFF,0xFF,0x3F,0xFE,0x7F,0xFE,0x00,0xFF,0xFF,
0xFF,0x3F,0xFE,0xFF,0xFF,0x80,0xFF,0xFF,0xFF,0x7F,0xFF,0xFF,0xFF,0x81,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xC3,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE3,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};                                                                             //汉字“需”字形码
static unsigned char nu[] U8G_PROGMEM = {
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC7,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0xFE,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0x0F,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0xFF,0xFF,0x3F,0xFE,0xFF,
0xFF,0xFF,0x0F,0xFF,0xFF,0x0F,0xF8,0xFF,0xFF,0xFF,0x8F,0xE7,0xFF,0x01,0xF0,0xFF,
0xFF,0xFF,0x8F,0xC7,0x1F,0x00,0xF0,0xFF,0xFF,0xFF,0xC7,0x87,0x07,0x1C,0xF8,0xFF,
0xFF,0xFF,0xC7,0x07,0x9F,0x1F,0xFC,0xFF,0xFF,0xFF,0xC7,0x87,0xFF,0x1F,0xFE,0xFF,
0xFF,0xFF,0x07,0x80,0xF3,0x1F,0xFF,0xFF,0xFF,0x7F,0x00,0xC0,0xC3,0x0F,0xFF,0xFF,
0x1F,0x00,0xE0,0xC3,0x87,0x8F,0xFF,0xFF,0x1F,0x00,0xF3,0xE3,0x1F,0x87,0xFF,0xFF,
0x7F,0xF0,0xF1,0xE1,0x3F,0xC4,0xFF,0xFF,0xFF,0xFF,0xF9,0xF1,0x7F,0xC0,0xFF,0xFF,
0xFF,0xFF,0xF8,0xF0,0xFF,0xE0,0xFF,0xFF,0xFF,0xFF,0xF8,0xF8,0xFF,0xC0,0xFF,0xFF,
0xFF,0xFF,0x61,0xF8,0x7F,0x00,0xFF,0xFF,0xFF,0xFF,0x03,0xFC,0x3F,0x0C,0xFC,0xFF,
0xFF,0xFF,0x07,0xFC,0x1F,0x1E,0xF8,0xFF,0xFF,0xFF,0x0F,0xFC,0x07,0x3F,0xC0,0xFF,
0xFF,0xFF,0x0F,0xF8,0xC3,0x7F,0x00,0xFF,0xFF,0xFF,0x07,0xF0,0xF0,0xFF,0x00,0xF0,
0xFF,0xFF,0xC3,0x20,0xFE,0xFF,0x01,0xC0,0xFF,0xFF,0xE0,0xE1,0xFF,0xFF,0x07,0xF8,
0xFF,0x7F,0xF8,0x23,0xFC,0xFF,0xFF,0xFF,0xFF,0x3F,0xFC,0x3F,0xF8,0xFF,0xFF,0xFF,
0xFF,0x8F,0xFF,0x3F,0xF8,0xFF,0xFF,0xFF,0xFF,0xF7,0xFF,0x3F,0xF8,0xF9,0xFF,0xFF,
0xFF,0xFF,0xFF,0x3F,0x3C,0xE0,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x00,0x80,0xFF,0xFF,
0xFF,0xFF,0xFF,0x01,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0xFE,0x83,0xFF,0xFF,
0xFF,0xFF,0x03,0x18,0xFE,0xC3,0xFF,0xFF,0xFF,0xFF,0xFF,0x1F,0xFE,0xC3,0xFF,0xFF,
0xFF,0xFF,0xFF,0x1F,0xFF,0xC3,0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0xFF,0xC3,0xFF,0xFF,
0xFF,0xFF,0xFF,0x8F,0xFF,0xE3,0xFF,0xFF,0xFF,0xFF,0xFF,0x87,0xFF,0xE3,0xFF,0xFF,
0xFF,0xFF,0xFF,0xC3,0xFF,0xE3,0xFF,0xFF,0xFF,0xFF,0xFF,0xC3,0xFF,0xE3,0xFF,0xFF,
0xFF,0xFF,0xFF,0xE1,0xFF,0xE1,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0xFF,0xE1,0xFF,0xFF,
0xFF,0xFF,0x7F,0xF8,0xFF,0xF1,0xFF,0xFF,0xFF,0xFF,0x3F,0xFC,0xFF,0xF1,0xFF,0xFF,
0xFF,0xFF,0x1F,0xFE,0xFF,0xF0,0xFF,0xFF,0xFF,0xFF,0x0F,0x7F,0xFF,0xF8,0xFF,0xFF,
0xFF,0xFF,0x87,0xFF,0x7C,0xF8,0xFF,0xFF,0xFF,0xFF,0xC3,0xFF,0x01,0xF8,0xFF,0xFF,
0xFF,0xFF,0xF0,0xFF,0x03,0xFC,0xFF,0xFF,0xFF,0x3F,0xF8,0xFF,0x03,0xFE,0xFF,0xFF,
0xFF,0x1F,0xFF,0xFF,0x07,0xFF,0xFF,0xFF,0xFF,0xE7,0xFF,0xFF,0x87,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xCF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};                                                                             //汉字“努”字形码
static unsigned char li[] U8G_PROGMEM = {
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x3F,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0xF0,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x3F,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0xF0,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x3F,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0xF8,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x3F,0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0xF8,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x3F,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0xFC,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x3F,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x1F,0xFE,0x87,0xFF,0xFF,
0xFF,0xFF,0xFF,0x1F,0xFE,0x01,0xFE,0xFF,0xFF,0xFF,0xFF,0x1F,0x1E,0x00,0xFC,0xFF,
0xFF,0xFF,0xFF,0x1F,0x00,0x00,0xF8,0xFF,0xFF,0xFF,0xFF,0x03,0x00,0x1F,0xF8,0xFF,
0xFF,0xBF,0x01,0x00,0xF8,0x1F,0xFC,0xFF,0xFF,0x3F,0x00,0x00,0xFF,0x1F,0xFC,0xFF,
0xFF,0x7F,0x00,0x84,0xFF,0x1F,0xFC,0xFF,0xFF,0xFF,0xE1,0x87,0xFF,0x1F,0xFE,0xFF,
0xFF,0xFF,0xFF,0x87,0xFF,0x1F,0xFE,0xFF,0xFF,0xFF,0xFF,0xC3,0xFF,0x1F,0xFE,0xFF,
0xFF,0xFF,0xFF,0xC3,0xFF,0x1F,0xFE,0xFF,0xFF,0xFF,0xFF,0xE1,0xFF,0x1F,0xFE,0xFF,
0xFF,0xFF,0xFF,0xE1,0xFF,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0xFF,0x0F,0xFF,0xFF,
0xFF,0xFF,0xFF,0xF0,0xFF,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0xFF,0x0F,0xFF,0xFF,
0xFF,0xFF,0x7F,0xF8,0xFF,0x0F,0xFF,0xFF,0xFF,0xFF,0x7F,0xFC,0xFF,0x0F,0xFF,0xFF,
0xFF,0xFF,0x3F,0xFC,0xFF,0x0F,0xFF,0xFF,0xFF,0xFF,0x1F,0xFE,0xFF,0x07,0xFF,0xFF,
0xFF,0xFF,0x1F,0xFE,0xFF,0x87,0xFF,0xFF,0xFF,0xFF,0x0F,0xFF,0xFF,0x87,0xFF,0xFF,
0xFF,0xFF,0x8F,0xFF,0xFF,0x87,0xFF,0xFF,0xFF,0xFF,0x87,0xFF,0xFF,0x83,0xFF,0xFF,
0xFF,0xFF,0xC3,0xFF,0xFF,0xC3,0xFF,0xFF,0xFF,0xFF,0xE3,0xFF,0xFF,0xC3,0xFF,0xFF,
0xFF,0xFF,0xE1,0xFF,0xFF,0xC1,0xFF,0xFF,0xFF,0xFF,0xF0,0xFF,0xFF,0xC1,0xFF,0xFF,
0xFF,0x7F,0xF8,0xFF,0xFF,0xE0,0xFF,0xFF,0xFF,0x3F,0xFC,0xFF,0xFF,0xE0,0xFF,0xFF,
0xFF,0x1F,0xFE,0xFF,0x7F,0xE0,0xFF,0xFF,0xFF,0x0F,0xFF,0x7F,0x7E,0xF0,0xFF,0xFF,
0xFF,0x8F,0xFF,0xFF,0x38,0xF0,0xFF,0xFF,0xFF,0xE3,0xFF,0xFF,0x00,0xF8,0xFF,0xFF,
0xFF,0xF1,0xFF,0xFF,0x01,0xF8,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0x03,0xFC,0xFF,0xFF,
0x7F,0xFF,0xFF,0xFF,0x07,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0xFE,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xCF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};                                                                             //汉字“力”字形码
static unsigned char youwin[] U8G_PROGMEM = {
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x7F,0x3F,0x1F,0xCF,0xE7,0xF3,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFC,0xF9,0xF3,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFE,0xFD,0xFB,0xF7,0x6F,0x9F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xDF,0x8F,0x07,0x73,0xF9,0xFD,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xF7,0xFB,0xFD,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFC,0xF9,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7D,0x7D,0x7D,
0x7D,0x7D,0x7D,0x7D,0x03,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7D,
0x83,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x83,0xFD,0xFD,0xFD,0xFD,0xFD,0x83,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x1F,0xEF,0xEF,
0xEF,0xEF,0xEF,0xEF,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x7F,0x7F,0x7F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x83,0x7D,0x7D,0x7D,0x7D,0x7D,0x83,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x78,0x77,0x77,
0x77,0x77,0x77,0x77,0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0x70,0xAF,0xDF,0xFF,0xFF,0xFE,0xFD,0xFB,0x77,0xAF,0xDF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0xF7,0xF7,0xF7,0xF7,0xF7,0x0F,0xFF,0xFF,0xFF,
0xFF,0xFF,0xF7,0xF7,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDF,0xDF,0xDF,
0xDF,0xDF,0xDF,0xDF,0xE0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xF8,0x77,0x77,0x77,0x77,0xFB,0xFD,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEE,0x5D,0xBD,0xBD,0xBD,0xBD,0xBC,0xFD,0xFD,0xFD,
0xFD,0xFD,0xFD,0xFD,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xBF,0xBF,0xBF,
0xBF,0xBF,0xBF,0xBF,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0x70,0xAF,0xDF,0xFF,0xFF,0xFE,0xFD,0xFB,0x77,0xAF,0xDF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x7F,0x7F,0x7F,0x7F,0x7F,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xBF,0x5F,0xEF,
0xF7,0xFB,0xFD,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xF8,0xF7,0xF7,0xF7,0xF7,0xFB,0xFD,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0xDF,0xDF,0xDF,0x9F,0x9F,0x20,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xBF,0xBF,0xBE,
0xBD,0xBB,0xB7,0xAF,0xDF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xE1,0x5E,0xBE,0xFE,0xFE,0xFD,0xFB,0xF7,0xEF,0x5F,0xBF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xBF,0xBF,0x0F,0xBF,0xBF,0xBF,0xBF,0xBF,0x7F,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEF,0xFF,0xEF,0xE7,0xEB,0xEF,
0xEF,0xEF,0xEF,0xEF,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xF0,0xEF,0xEF,0xEF,0xEF,0xF7,0xFB,0xFD,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x7F,0x7F,0x7F,0x7F,0x7B,0xFC,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x1F,0xEF,0xEF,0xEF,
0xEF,0xEF,0xEF,0xEE,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0xEF,0xEF,0xE0,0xFF,0xEF,0xF0,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3C,0x3B,0x3B,0x3B,
0x3B,0x3B,0xFB,0x3B,0x3B,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC9,0xC9,0xC9,0xC9,0xC9,0xC9,0xC9,0xC9,0xC9,0xC9,
0xC9,0xC9,0xFF,0xC9,0xC9,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};                                                                             //图形“你赢了”图形码

void stop() {
  u8g.undoRotation();
  u8g.firstPage();
  do {
    setFont_M;
    u8g.setPrintPos(0, 30);
    u8g.print("Welcome to the");
    setFont_M;
    u8g.setPrintPos(0, 60);
    u8g.print("BINGO GAME");
  } while( u8g.nextPage() );
  while(!joy_stick(A2,0));
  score = 0;
  timer2lastTime = millis();
  timer1lastTime = millis();
}                             //定义“stop”函数

void start() {
  if (bingo == 1 || miss == 1) {
    while((a == a_random)) {
      a_random = random(1, 7);

    }
    a = a_random;
    bingo = 0;
    miss = 0;
    timer1lastTime = millis();

  }
}                                //定义“start”函数

void display() {
  b = (map(analogRead(A0), 1023, 0, 0, 90));
  u8g.undoRotation();
  u8g.firstPage();
  do {
    setFont_M;
    u8g.setPrintPos(60, 20);
    u8g.print(a);
    setFont_M;
    u8g.setPrintPos((60 - b), 40);
    u8g.print("1 2 3 4 5 6");
    setFont_M;
    u8g.setPrintPos(20, 60);
    u8g.print("SCORE:");
    setFont_M;
    u8g.setPrintPos(80, 60);
    u8g.print(score);
    setFont_M;
    u8g.setPrintPos(100, 20);
    u8g.print(time1);
    setFont_M;
    u8g.setPrintPos(0, 20);
    u8g.print(time2);
    setFont_M;
    u8g.setPrintPos(20, 20);
    u8g.print("s");
    setFont_M;
    u8g.setPrintPos(110, 20);
    u8g.print("s");
    setFont_S;
    u8g.setPrintPos(88, 64);
    u8g.print("top");
    setFont_S;
    u8g.setPrintPos(112, 64);
    u8g.print(top);
  } while( u8g.nextPage() );
}                           //定义“display”函数

void win() {
  if (score == 15) {
    time2 = (millis() - timer2lastTime);
    time2 = time2 / 1000;
    top = EEPROM.read(0);
    if (time2 < top) {
      u8g.undoRotation();
      u8g.firstPage();
      do {
        setFont_L;
        u8g.drawXBMP( 0, 0, 128, 64, youwin);     
      } while( u8g.nextPage() );
      delay(2000);                                       //输出图形“你赢了”图形，并延迟两秒
      u8g.undoRotation();
      u8g.firstPage();
      do {
        setFont_M;
        u8g.setPrintPos(0, 30);
        u8g.print("you are best!");
        setFont_L;
        u8g.setPrintPos(30, 60);
        u8g.print(time2);
      } while( u8g.nextPage() );
      EEPROM.write(0, time2);
      for (int a = 0; music_6[a] != 0; a++) {
        if (music_6[a] != 22) {
          tone(BuzzerPin8, tone_list[music_6[a] - 1]);
        }
        else {
          noTone(BuzzerPin8);
        }
        delay(rhythm_6[a] * 300);
        noTone(BuzzerPin8);
        delay(30);
      }
      delay(1000);

    } else {
      u8g.undoRotation();
      u8g.firstPage();
      do {
        setFont_L;
        u8g.drawXBMP( 32, 0, 64, 64, hai);     
      } while( u8g.nextPage() );
      delay(1000);                         //输出汉字“还”字形，并延迟一秒
      u8g.undoRotation();
      u8g.firstPage();
      do {
        setFont_L;
        u8g.drawXBMP( 32, 0, 64, 64, xu);     
      } while( u8g.nextPage() );
      delay(1000);                         //输出汉字“需”字形，并延迟一秒
      u8g.undoRotation();
      u8g.firstPage();
      do {
        setFont_L;
        u8g.drawXBMP( 32, 0, 64, 64, nu);    
      } while( u8g.nextPage() );
      delay(1000);                         //输出汉字“努”字形，并延迟一秒
       u8g.undoRotation();
       u8g.firstPage();
      do {    
        setFont_L;
        u8g.drawXBMP( 32, 0, 64, 64, li);    
      } while( u8g.nextPage() );
      delay(1000);                         //输出汉字“力”字形，并延迟一秒
      u8g.undoRotation();
      u8g.firstPage();
      do {
        setFont_L;
        u8g.setPrintPos(30, 40);
        u8g.print(time2);
      } while( u8g.nextPage() );
      strip.setPixelColor(1-1, 51,204,0);
      strip.show();
      delay(1000);                         
      strip.setPixelColor(1-1, 0,0,0);
      strip.show();

    }
    while(!joy_stick(A2,0));
    score = 0;
    timer2lastTime = millis();

  }
}                                       //定义“win”函数

void BINGO() {
  if ((b + 27) / 18 == a) {
    hold = analogRead(A0);
    delay(100);
    if (analogRead(A0) == hold) {
      bingo = 1;
      score = score + 1;
      tone(BuzzerPin8,2000);
      delay(20);
      noTone(BuzzerPin8);

    }

  }
}                                    //定义“bingo”函数

void time_out() {
  if ((millis() - timer1lastTime) > 2000) {              //设置每个随机数出现时间
    strip.setPixelColor(1-1, 255,0,0);
    strip.show();
    tone(BuzzerPin8,200);
    delay(20);
    strip.setPixelColor(1-1, 0,0,0);
    strip.show();
    noTone(BuzzerPin8);
    miss = 1;

  }
}                                      //定义“time_out”函数

void game_over() {
  if ((millis() - timer2lastTime) > 60000) {          //设定游戏终止时间为60秒
    u8g.undoRotation();
    u8g.firstPage();
    do {
      setFont_L;
      u8g.setPrintPos(0, 30);
      u8g.print("GAME");
      setFont_L;
      u8g.setPrintPos(40, 60);
      u8g.print("OVER");
      tone(BuzzerPin8,262);
      delay(2000);
      noTone(BuzzerPin8);
    } while( u8g.nextPage() );
    while(!joy_stick(A2,0));
    score = 0;
    timer2lastTime = millis();

  }
}                                          //定义“game_over”函数

void setup()
{
  AUDIO.init(DEVICE_TF,MODE_RAM,20);      //设置音乐模块参数
AUDIO.choose(1);
AUDIO.pause();

  strip.begin();
  strip.show();
  a = 0;
  a_random = 0;
  b = 0;
  score = 0;
  bingo = 1;
  hold = 0;
  miss = 0;
  time1 = 0;
  time2 = 0;
  top = 0;
  for (int a = 0; music_3[a] != 0; a++) {
    if (music_3[a] != 22) {
      tone(BuzzerPin8, tone_list[music_3[a] - 1]);
    }
    else {
      noTone(BuzzerPin8);
    }
    delay(rhythm_3[a] * 300);
    noTone(BuzzerPin8);
    delay(30);
  }
  delay(1000);
  u8g.undoRotation();
  u8g.firstPage();
  do {
    setFont_M;
    u8g.setPrintPos(0, 30);
    u8g.print("Welcome to the");
    setFont_M;
    u8g.setPrintPos(0, 60);
    u8g.print("BINGO GAME");
  } while( u8g.nextPage() );
  while(!joy_stick(A2,0));
  top = EEPROM.read(0);
  u8g.undoRotation();
  u8g.firstPage();
  do {
    setFont_L;
    u8g.setPrintPos(30, 30);
    u8g.print("top");
    setFont_M;
    u8g.setPrintPos(40, 60);
    u8g.print(top);
  } while( u8g.nextPage() );
  while(!joy_stick(A2,1));
  timer2lastTime = millis();
  AUDIO.play();
}                               //初始化

void loop()
{
  time2 = (millis() - timer2lastTime);
  time1 = (millis() - timer1lastTime);
  time2 = time2 / 1000;
  time1 = time1 / 1000;
  if (joy_stick(A2,3)) {               //遥感向上按
    stop();

  } else {
    start();
    display();
    BINGO();
    win();
    time_out();
    game_over();

  }
  if (joy_stick(A2,2)) {                      //遥感向左按
    AUDIO.pause();
  } else if (joy_stick(A2,4)) {               //遥感向右按
    AUDIO.play();}

}
