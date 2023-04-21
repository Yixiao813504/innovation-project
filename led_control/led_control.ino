#include <Arduino.h>
#include <FastLED.h>                
#include <LiquidCrystal_I2C.h>      
#include <Wire.h>                   
#include <DHT.h>                    



#define NUM_LEDS 60                 
#define LED_TYPE WS2812B            
#define COLOR_ORDER GRB             
#define LED_PIN1 8                  
#define LED_PIN2 9                  
#define LED_PIN3 10                 
#define LED_PIN4 11                 
CRGB leds1[NUM_LEDS];               
CRGB leds2[NUM_LEDS];                     
CRGB leds3[NUM_LEDS];                    
CRGB leds4[NUM_LEDS];               
int brightness = 120;               
int leds_count = 0;                 

int ir_pin = 2;                     
float temperature = 0;              
float humidity = 0;                 


void leds_display();                
boolean ir_detect();                

void setup()
{
  Serial.begin(9600);

  pinMode(ir_pin, INPUT);           
  LEDS.addLeds<LED_TYPE, LED_PIN1, COLOR_ORDER>(leds1, NUM_LEDS);   
  LEDS.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(leds2, NUM_LEDS);   
  LEDS.addLeds<LED_TYPE, LED_PIN3, COLOR_ORDER>(leds3, NUM_LEDS);   
  LEDS.addLeds<LED_TYPE, LED_PIN4, COLOR_ORDER>(leds4, NUM_LEDS);   
  
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds1[i] = CRGB::Black;
    leds2[i] = CRGB::Black;
    leds3[i] = CRGB::Black;
    leds4[i] = CRGB::Black;
  }
  FastLED.setBrightness(brightness);  
  FastLED.show();                     
}

void loop() 
{
  leds_display();                     

}




void leds_display()
{
  leds1[0] = CRGB (brightness,brightness,brightness);   
  leds2[0] = CRGB (brightness,brightness,brightness);   
  leds3[0] = CRGB (brightness,brightness,brightness);   
  leds4[0] = CRGB (brightness,brightness,brightness);   
  FastLED.show();                   
 
  for(int i = 0 ;i < NUM_LEDS; i++)
  {
    leds1[NUM_LEDS-1-i] = leds1[NUM_LEDS-2-i];
    leds2[NUM_LEDS-1-i] = leds2[NUM_LEDS-2-i];
    leds3[NUM_LEDS-1-i] = leds3[NUM_LEDS-2-i];
    leds4[NUM_LEDS-1-i] = leds4[NUM_LEDS-2-i];
  }
  brightness -= 5;                  
  if(brightness <= 0)               
  {
    brightness = 0;                 
    leds_count++;                   
    if(leds_count == 60)            
    {
      if(ir_detect())               
      {
        brightness = 120;           
      }
      else                                           
      {
        for(int i = 0; i < NUM_LEDS; i++)   
        {
          leds1[i] = CRGB::Black;
          leds2[i] = CRGB::Black;
          leds3[i] = CRGB::Black;
          leds4[i] = CRGB::Black;
        }
        FastLED.show();                     

        Serial.println("Waiting...");
        
        while(ir_detect() == false)
        {}
      }
      leds_count = 0;               
    }
    
  }
  delay(20);                        
}


boolean ir_detect()
{
  int ir_value = digitalRead(ir_pin); 
  if(ir_value == HIGH)                
  {
    return true;                      
  }
  else                                
  {
    return false;                     
  }
}
