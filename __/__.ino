/*
1-使用4条ws2812灯带，一条灯带60颗灯珠，实现流星雨效果
2-DHT11温湿度传感器，检测温度和湿度
3-温湿度数据显示在IIC1602液晶屏上
4-使用一个红外传感器，当检测到人时，4条灯带同时亮起，当人离开时，灯带熄灭
*/

#include <Arduino.h>
#include <FastLED.h>                // 导入FastLED库
#include <LiquidCrystal_I2C.h>      // 导入LCD库
#include <Wire.h>                   // 导入Wire库
#include <DHT.h>                    // 导入DHT库

// Some comment

#define NUM_LEDS 60                 // 灯带上的灯珠数量
#define LED_TYPE WS2812B            // 灯带类型
#define COLOR_ORDER GRB             // 灯带颜色顺序
#define LED_PIN1 8                  // 灯带1引脚8
#define LED_PIN2 9                  // 灯带2引脚9
#define LED_PIN3 10                 // 灯带3引脚10
#define LED_PIN4 11                 // 灯带4引脚11
CRGB leds1[NUM_LEDS];               // 初始化灯带1
CRGB leds2[NUM_LEDS];               // 初始化灯带2       
CRGB leds3[NUM_LEDS];               // 初始化灯带3     
CRGB leds4[NUM_LEDS];               // 初始化灯带4
int brightness = 120;               // 灯带亮度
int leds_count = 0;                 // 灯带计数

int ir_pin = 2;                     // 红外传感器引脚
float temperature = 0;              // 温度变量初始化
float humidity = 0;                 // 湿度变量初始化


void leds_display();                // 显示流星雨效果
boolean ir_detect();                // 红外传感器检测

void setup()
{
  Serial.begin(9600);

  pinMode(ir_pin, INPUT);           // 设置红外传感器引脚为输入模式
  LEDS.addLeds<LED_TYPE, LED_PIN1, COLOR_ORDER>(leds1, NUM_LEDS);   // 初始化灯带1
  LEDS.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(leds2, NUM_LEDS);   // 初始化灯带2
  LEDS.addLeds<LED_TYPE, LED_PIN3, COLOR_ORDER>(leds3, NUM_LEDS);   // 初始化灯带3
  LEDS.addLeds<LED_TYPE, LED_PIN4, COLOR_ORDER>(leds4, NUM_LEDS);   // 初始化灯带4
  // 初始化灯带为黑色
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds1[i] = CRGB::Black;
    leds2[i] = CRGB::Black;
    leds3[i] = CRGB::Black;
    leds4[i] = CRGB::Black;
  }
  FastLED.setBrightness(brightness);  // 设置灯带亮度
  FastLED.show();                     // 显示灯带
}

void loop() 
{
  leds_display();                     // 显示流星雨效果

}



// 显示流星雨效果
void leds_display()
{
  leds1[0] = CRGB (brightness,brightness,brightness);   // 设置灯带1第一颗灯的颜色
  leds2[0] = CRGB (brightness,brightness,brightness);   // 设置灯带2第一颗灯的颜色
  leds3[0] = CRGB (brightness,brightness,brightness);   // 设置灯带3第一颗灯的颜色
  leds4[0] = CRGB (brightness,brightness,brightness);   // 设置灯带4第一颗灯的颜色
  FastLED.show();                   // 显示灯带
  // 将灯带的灯向后移动一位
  for(int i = 0 ;i < NUM_LEDS; i++)
  {
    leds1[NUM_LEDS-1-i] = leds1[NUM_LEDS-2-i];
    leds2[NUM_LEDS-1-i] = leds2[NUM_LEDS-2-i];
    leds3[NUM_LEDS-1-i] = leds3[NUM_LEDS-2-i];
    leds4[NUM_LEDS-1-i] = leds4[NUM_LEDS-2-i];
  }
  brightness -= 5;                  // 灯带亮度减小
  if(brightness <= 0)               // 如果灯带亮度小于等于0
  {
    brightness = 0;                 // 灯带亮度为0
    leds_count++;                   // 灯带计数加1
    if(leds_count == 60)            // 如果灯带计数等于60
    {
      if(ir_detect())               // 如果检测到人
      {
        brightness = 120;           // 灯带亮度为120
      }
      else                                  // 如果没有检测到人         
      {
        for(int i = 0; i < NUM_LEDS; i++)   // 设置灯带为黑色
        {
          leds1[i] = CRGB::Black;
          leds2[i] = CRGB::Black;
          leds3[i] = CRGB::Black;
          leds4[i] = CRGB::Black;
        }
        FastLED.show();                     // 显示灯带

        Serial.println("Waiting...");
        // 如果没有检测到人，一直等待
        while(ir_detect() == false)
        {}
      }
      leds_count = 0;               // 灯带计数清零
    }
    
  }
  delay(20);                        // 延时20ms
}

// 红外传感器检测
boolean ir_detect()
{
  int ir_value = digitalRead(ir_pin); // 读取红外传感器引脚的值
  if(ir_value == HIGH)                // 如果红外传感器引脚的值为高电平
  {
    return true;                      // 返回true
  }
  else                                // 如果红外传感器引脚的值为低电平
  {
    return false;                     // 返回false
  }
}
