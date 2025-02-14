//这是收音机的示例程序，由频率值生成5个控制字节，发送给ICMaker_TEA5767模块

#include <Wire.h>

 unsigned char frequencyH = 0;//存放频率高8位
 unsigned char frequencyL = 0;//存放频率高8位

 unsigned int frequencyB;//用于频率熟据类型的转换
 double frequency = 0;   //频率变量为双精度

 void setup()
 {
   Wire.begin();
   frequency = 88.7; //starting frequency设定初始频率
   Serial.begin(9600);
 }

 void loop()
 {
   frequencyB = 4 * (frequency * 1000000 + 225000) / 32768;//数据类型的转换
   frequencyH = frequencyB >> 8;//右移8位，左补0
   frequencyL = frequencyB & 0XFF;//按位跟int型数据与运算，舍弃高8位，0xff：0000 0000 1111 1111
   delay(100);
   Wire.beginTransmission(0x60);//ICMaker_TEA5767的地址
   Wire.write(frequencyH);//以下写入5个字节的十六进制数
   Wire.write(frequencyL);
   Wire.write(0xB0);
   Wire.write(0x10);
   Wire.write((byte)0x00);
   Wire.endTransmission();
   delay(100); 
   Serial.println(frequency);
 }
