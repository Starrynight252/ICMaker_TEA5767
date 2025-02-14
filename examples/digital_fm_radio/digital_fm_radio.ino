/*         Arduino Digital FM Radio with build in speakers
  libraries & schematic: www.ardumotive.com
       dev: Michalis Vasilakis // Date 24/2/2015 // Version 1.0        
              Many thanks to nicuflorica.blogspot.ro                       */

//Libraries:
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <ICMaker_TEA5767.h>
#include <Wire.h>
#include <Button.h>

//Constants:
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3 ); //Pinout:(SCLK, DIN, DC, CS, RST)
ICMaker_TEA5767 Radio; //Pinout SLC and SDA - Arduino pins A5 and A4
Button btn_forward(11, HIGH); //Search station up button
Button btn_backward(12, HIGH);//Search station down button

//Variables:
double old_frequency;
double frequency;
int search_mode = 0;
int search_direction;
unsigned long last_pressed;
unsigned char buf[5];
int stereo;
int signal_level;
double current_freq;
unsigned long current_millis = millis();


void setup () {
  //Init
  Wire.begin();
  Radio.init();
  Radio.set_frequency(95.2); //On power on go to station 95.2
  display.begin();
  display.setContrast(100);
  display.clearDisplay();
}

void loop () {

  if (Radio.read_status(buf) == 1) {
    current_freq =  floor (Radio.frequency_available (buf) / 100000 + .5) / 10;
    stereo = Radio.stereo(buf);
    signal_level = Radio.signal_level(buf);

   display.setTextSize(2);
   display.setTextColor(BLACK);
   display.setCursor(0,0);
   display.print(display.print(current_freq));
   display.setCursor(47,15);
   display.print("MHz");
   display.setCursor(0,20);
   display.setTextSize(1);
   display.setTextColor(BLACK);
   //Strereo or mono ?
   if (stereo) display.print("STEREO"); 
   else display.print("MONO");
   // display level of FM signal..
   display.setCursor(0,35);
   display.setTextSize(1);
   display.setTextColor(WHITE, BLACK);
   display.print(signal_level);
   display.print("/15 ");
   display.display();
   delay (500);
   display.clearDisplay(); 
   //Draw a signal level triangle...
   display.drawLine(80, 30, 80, 45, BLACK);
   display.drawLine(80, 45, 50, 45, BLACK);
   display.drawLine(50, 45, 80, 30, BLACK);
   //Fill triangle with signal strength
   int sl = signal_level;
   for (int x = 0; x < sl; x++)
   { 
   display.drawLine(50+2*x, 45, 50+2*x, 45-x, BLACK);
   }
  }
  //When button pressed, search for new station
  if (search_mode == 1) {
      if (Radio.process_search (buf, search_direction) == 1) {
          search_mode = 0;
      }
  }
  //If forward button is pressed, go up to next station
  if (btn_forward.isPressed()) {
    last_pressed = current_millis;
    search_mode = 1;
    search_direction = ICMaker_TEA5767_SEARCH_DIR_UP;
    Radio.search_up(buf);
    delay(1000);
  }
  //If backward button is pressed, go down to next station
  if (btn_backward.isPressed()) {
    last_pressed = current_millis;
    search_mode = 1;
    search_direction = ICMaker_TEA5767_SEARCH_DIR_DOWN;
    Radio.search_down(buf);
    delay(1000);
  } 
   delay(100);
}
