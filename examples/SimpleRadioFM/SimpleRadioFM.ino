#include <ICMaker_TEA5767.h>
#include <Wire.h>

ICMaker_TEA5767 Radio;

double old_frequency;
double frequency;
int search_mode = 0;
int search_direction;
unsigned long last_pressed;

void setup() { 
pinMode(2, INPUT); 
pinMode(3, INPUT);
  Wire.begin();
  Radio.init();
  Radio.set_frequency(105.4); 
}

void loop() {

  unsigned char buf[5]; //5个控制字符
  double current_freq;  //当前频率
  unsigned long current_millis = millis();
  if (Radio.read_status(buf) == 1) {
    current_freq =  floor (Radio.frequency_available (buf) / 100000 + .5) / 10;

  }
  
  if (search_mode == 1) {
      if (Radio.process_search (buf, search_direction) == 1) {
          search_mode = 0;
      }
  }
  
  if (digitalRead(2)==0){
    last_pressed = current_millis;
    search_mode = 1;
    search_direction = ICMaker_TEA5767_SEARCH_DIR_UP;
    Radio.search_up(buf);
    delay(300);
  }
  
  if (digitalRead(3)==0) {
    last_pressed = current_millis;
    search_mode = 1;
    search_direction = ICMaker_TEA5767_SEARCH_DIR_DOWN;
    Radio.search_down(buf);
    delay(300);
  } 
  //delay(20); 
  delay(50);
}