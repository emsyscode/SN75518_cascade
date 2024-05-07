/**********************************************************************/
/*This code is not clean and OFFCOURSE will be possible do it better! */
/*this is only a group of functions to be used as independent code,   */
/*and by this way will be possible to any person can do the changes   */
/*of code and see what happen.                                        */
/*The code don't depend of any external library or extenal functions  */
/*complicated.                                                        */
/*I'm let the values to sent as binnary, this allow swap bit by bit   */
/* to is possible test segment by segment without convert to HEX      */
/**********************************************************************/
/* This code show how is possible work with the driver SN75518        */
/*                                                                    */
/**********************************************************************/
#include <Arduino.h>
#include <String.h>

#define VFD_in 7  // This is the pin number 7 on Arduino UNO
#define VFD_clk 8 // This is the pin number 8 on Arduino UNO
#define VFD_stb1 9 // This is the pin number 9 on Arduino UNO
#define VFD_stb2 10 // This is the pin number 10 on Arduino UNO
#define VFD_latchBoth 12//This is the pin number 12 on Arduino UNO

int ledPin = 13; // LED connected to digital pin 13
// don't define the other pins, but is A0 to input analog signal, pins D2 & D3 is trigger input,

//ATT: On the Uno and other ATMEGA based boards, unsigned ints (unsigned integers) are the same as ints in that they store a 2 byte value.
//Long variables are extended size variables for number storage, and store 32 bits (4 bytes), from -2,147,483,648 to 2,147,483,647.
unsigned char word0 = 0x00;
unsigned char word1 = 0x00;
unsigned char word2 = 0x00;
unsigned char word3 = 0x00;
unsigned char word4 = 0x00;
unsigned char word5 = 0x00;
unsigned char word6 = 0x00;
unsigned char word7 = 0x00;
bool flag = true;
//*************************************************//
void send_char(unsigned char a) {
  unsigned char transmit = 15; //define our transmit pin
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  data = a;
  // the validation of data happen when clk go from LOW to HIGH.
  digitalWrite(VFD_stb1, LOW); // When strobe is low, all output is enable. If high, all output will be set to low.
  digitalWrite(VFD_stb2, LOW);
  delayMicroseconds(5);
  digitalWrite(VFD_clk, LOW); // need invert the signal to allow 8 bits is is low only send 7 bits
  delayMicroseconds(5);
  for (mask = 0b10000000; mask > 0; mask >>= 1) { //iterate through bit mask
    digitalWrite(VFD_clk, LOW); // need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
    if (data & mask) { // if bitwise AND resolves to true
      digitalWrite(VFD_in, HIGH);
      //Serial.print(1);
    }
    else { //if bitwise and resolves to false
      digitalWrite(VFD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(VFD_clk, HIGH); // need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
    //
    digitalWrite(VFD_stb1, HIGH); // When strobe is low, all output is enable. If high, all output will be set to low.
    digitalWrite(VFD_stb2, HIGH);
    delayMicroseconds(5);
  }
}
void send_char_without(unsigned char a){
  unsigned char transmit = 15; //define our transmit pin
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  data = a;
  for (mask = 0b10000000; mask > 0; mask >>= 1) { //iterate through bit mask
    digitalWrite(VFD_clk, LOW);
    delayMicroseconds(5);
    if (data & mask) { // if bitwise AND resolves to true
      digitalWrite(VFD_in, HIGH);
      //Serial.print(1);
    }
    else { //if bitwise and resolves to false
      digitalWrite(VFD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(VFD_clk, HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
  }
  //Serial.println();
}
void allON_SN75518_grid_1() {
  //I'm using 2 times a sn75518
  //send total of 45 bits (6Bytes), the last 3 bits are not used!
  digitalWrite(VFD_stb1, HIGH); //
  
  delayMicroseconds(1);
  
  send_char_without(0B11111001);  send_char_without(0B11111100); //   0:7   -   8:15//    byte 0 & 1
  send_char_without(0B00000000);  send_char_without(0B00000100); //  16:23  -  24:31//    Byte 2 & 3 // The first 3 bits of byte 3 belongs to GRID's
 //Clock grid is 32, center grid is 31, left grid is 30.
 //13&14 is a smicolon and segments of 2º grid
  send_char_without(0B10000111);  send_char_without(0B11110011); //  32:39  -  40:47//    Byte 4 & 5
  send_char_without(0B00011111);  send_char_without(0B11111111); //  48:55  -  56:63//    Byte 6 & 7


  digitalWrite(VFD_latchBoth, HIGH); //
  delayMicroseconds(1);
  digitalWrite(VFD_latchBoth, LOW); //
  delayMicroseconds(1);
  digitalWrite(VFD_stb1, LOW); //
}
void allON_SN75518_grid_2() {
  //I'm using 2 times a sn75518
  //send total of 45 bits (6Bytes), the last 3 bits are not used!
  digitalWrite(VFD_stb1, HIGH); //
  
  delayMicroseconds(1);

  send_char_without(0B00000000);  send_char_without(0B00000000); //   7:0   -   15:8//    byte 0 & 1
  send_char_without(0B00000000);  send_char_without(0B00000010); //  23:16  -  31:25//    Byte 2 & 3 //The first 3 bits of byte 3 belongs to grid's 1,2 & 3
 //Clock grid is 32, center grid is 31, left grid is 30.
 //13&14 is a smicolon and segments of 2º grid
  send_char_without(0B11111111);  send_char_without(0B11111111); //  39:32  -  47:40//    Byte 4 & 5
  send_char_without(0B11111111);  send_char_without(0B11111111); //  55:48  -  63:56//    Byte 6 & 7


  digitalWrite(VFD_latchBoth, HIGH); //
  delayMicroseconds(1);
  digitalWrite(VFD_latchBoth, LOW); //
  delayMicroseconds(1);
  digitalWrite(VFD_stb1, LOW); //
}
void allON_SN75518_grid_3() {
  //I'm using 2 times a sn75518
  //send total of 45 bits (6Bytes), the last 3 bits are not used!
  digitalWrite(VFD_stb1, HIGH); //
  
  delayMicroseconds(1);

  send_char_without(0B11111111);  send_char_without(0B11111111); //   1:8   -   9:16//    byte 0 & 1
  send_char_without(0B11111111);  send_char_without(0B11111001); //  17:24  -  25:32//    Byte 2 & 3
 //Clock grid is 32, center grid is 31, left grid is 30.
 //13&14 is a smicolon and segments of 2º grid
  send_char_without(0B11111111);  send_char_without(0B11111111); //  33:40  -  41:48//    Byte 4 & 5
  send_char_without(0B11111111);  send_char_without(0B11111111); //  49:56  -  57:64//    Byte 6 & 7

  digitalWrite(VFD_latchBoth, HIGH); //
  delayMicroseconds(1);
  digitalWrite(VFD_latchBoth, LOW); //
  delayMicroseconds(1);
  digitalWrite(VFD_stb1, LOW); //
}
void allON_SN75518_withoutSymbols() {
  //I'm using 2 times a sn75518
  //send total of 45 bits (6Bytes), the last 3 bits are not used!
  digitalWrite(VFD_stb1, HIGH); //
  
  delayMicroseconds(1);

  send_char_without(0B11111001);  send_char_without(0B11111100); //   1:8   -   9:16//    byte 0 & 1
  send_char_without(0B00000000);  send_char_without(0B00000110); //  17:24  -  25:32//    Byte 2 & 3
 //Clock grid is 32, center grid is 31, left grid is 30.
 //13&14 is a smicolon and segments of 2º grid
  send_char_without(0B11111111);  send_char_without(0B11111111); //  33:40  -  41:48//    Byte 4 & 5
  send_char_without(0B00111111);  send_char_without(0B11111111); //  49:56  -  57:64//    Byte 6 & 7


  digitalWrite(VFD_latchBoth, HIGH); //
  delayMicroseconds(1);
  digitalWrite(VFD_latchBoth, LOW); //
  delayMicroseconds(1);
  digitalWrite(VFD_stb1, LOW); //
}
void allON_SN75518() {
  //I'm using 2 times a sn75518
  //send total of 45 bits (6Bytes), the last 3 bits are not used!
  digitalWrite(VFD_stb1, HIGH); //
  
  delayMicroseconds(1);

  send_char_without(0B11111111);  send_char_without(0B11111111); //   1:8   -   9:16//    byte 0 & 1
  send_char_without(0B11111111);  send_char_without(0B11111111); //  17:24  -  25:32//    Byte 2 & 3

  send_char_without(0B11111111);  send_char_without(0B11111111); //  33:40  -  41:48//    Byte 4 & 5
  send_char_without(0B11111111);  send_char_without(0B11111111); //  49:56  -  57:64//    Byte 6 & 7


  digitalWrite(VFD_latchBoth, HIGH); //
  delayMicroseconds(1);
  digitalWrite(VFD_latchBoth, LOW); //
  delayMicroseconds(1);
  digitalWrite(VFD_stb1, LOW); //
}
void allON_SN75518_clock() {
  //I'm using 2 times a sn75518
  //send total of 45 bits (6Bytes), the last 3 bits are not used!
  digitalWrite(VFD_stb1, HIGH); //
  
  delayMicroseconds(1);

  send_char_without(0B00000000);  send_char_without(0B00000000); //   1:8   -   9:16//    byte 0 & 1
  send_char_without(0B00000000);  send_char_without(0B00000001); //  17:24  -  25:32//    Byte 2 & 3
 //Grids do clock é o 32! bytes são ponteiros do Q1
 //
  send_char_without(0B00000001);  send_char_without(0B00000001); //  33:40  -  41:48//    Byte 4 & 5
  send_char_without(0B00000000);  send_char_without(0B00000000); //  49:56  -  57:64//    Byte 6 & 7


  digitalWrite(VFD_latchBoth, HIGH); //
  delayMicroseconds(1);
  digitalWrite(VFD_latchBoth, LOW); //
  delayMicroseconds(1);
  digitalWrite(VFD_stb1, LOW); //
}
void msgHELLO_grid_1() {
  //I'm using 2 times a sn75518
  //send total of 45 bits (6Bytes), the last 3 bits are not used!
  digitalWrite(VFD_stb1, HIGH); //
  
  delayMicroseconds(1);

  send_char_without(0B10000001);  send_char_without(0B11011100); //   7:0   -   15:8//    byte 0 & 1
  send_char_without(0B00000000);  send_char_without(0B00000100); //  23:16  -  31:25//    Byte 2 & 3 //The first 3 bits of byte 3 belongs to grid's 1,2 & 3
 //Clock grid is 32, center grid is 31, left grid is 30.
 //13&14 is a smicolon and segments of 2º grid
  send_char_without(0B10000111);  send_char_without(0B00000011); //  39:32  -  47:40//    Byte 4 & 5
  send_char_without(0B00010111);  send_char_without(0B01101101); //  55:48  -  63:56//    Byte 6 & 7


  digitalWrite(VFD_latchBoth, HIGH); //
  delayMicroseconds(1);
  digitalWrite(VFD_latchBoth, LOW); //
  delayMicroseconds(1);
  digitalWrite(VFD_stb1, LOW); //
}
void msgPEOPLE_grid_1() {
  digitalWrite(VFD_stb1, HIGH); //
  delay(1);

  send_char_without(0B01101001);  send_char_without(0B11000000); //   7:0   -   15:8//    byte 0 & 1
  send_char_without(0B00000000);  send_char_without(0B00000100); //  23:16  -  31:25//    Byte 2 & 3 //The first 3 bits of byte 3 belongs to grid's 1,2 & 3
 //Clock grid is 32, center grid is 31, left grid is 30.
 //13&14 is a smicolon and segments of 2º grid
  send_char_without(0B10000111);  send_char_without(0B01110011); //  39:32  -  47:40//    Byte 4 & 5
  send_char_without(0B00011111);  send_char_without(0B01100101); //  55:48  -  63:56//    Byte 6 & 7

  digitalWrite(VFD_latchBoth, HIGH); //
  delay(1);
  digitalWrite(VFD_latchBoth, LOW); //
  delay(1);
  digitalWrite(VFD_stb1, LOW); //
}
void msgPEOPLE_grid_2() {
  digitalWrite(VFD_stb1, HIGH); //
  delay(1);

  send_char_without(0B00000000);  send_char_without(0B00000000); //   7:0   -   15:8//    byte 0 & 1
  send_char_without(0B00000000);  send_char_without(0B00000010); //  23:16  -  31:25//    Byte 2 & 3 //The first 3 bits of byte 3 belongs to grid's 1,2 & 3
 //Clock grid is 32, center grid is 31, left grid is 30.
 //13&14 is a smicolon and segments of 2º grid
  send_char_without(0B00001010);  send_char_without(0B00100101); //  39:32  -  47:40//    Byte 4 & 5
  send_char_without(0B00000000);  send_char_without(0B00000000); //  55:48  -  63:56//    Byte 6 & 7
  
  digitalWrite(VFD_latchBoth, HIGH); //
  delay(1);
  digitalWrite(VFD_latchBoth, LOW); //
  delay(1);
  digitalWrite(VFD_stb1, LOW); //
}
void animation_clock(byte word0,byte word1,byte word2,byte word3,byte word4,byte word5,byte word6,byte word7) {
  //I'm using 2 times a sn75518, but only use the 45 bits to connect to the VFD from AIWA(I think is the brand of compact HIFI with the VFD)
  //send total of 45 bits (6Bytes), the last 3 bits are not used!
  digitalWrite(VFD_stb1, HIGH); //
  
  delayMicroseconds(1);

  send_char_without(word0);  send_char_without(word1); //   1:8   -   9:16//    byte 0 & 1
  send_char_without(word2);  send_char_without(word3 = word3 | 0x01); //  17:24  -  25:32//    Byte 2 & 3
 //Grids of clock is bit nº:32! The remaning bits are pointrs of clock.
 //
  send_char_without(word4);  send_char_without(word5); //  33:40  -  41:48//    Byte 4 & 5
  send_char_without(word6);  send_char_without(word7); //  49:56  -  57:64//    Byte 6 & 7

  digitalWrite(VFD_latchBoth, HIGH); //
  delayMicroseconds(1);
  digitalWrite(VFD_latchBoth, LOW); //
  delayMicroseconds(1);
  digitalWrite(VFD_stb1, LOW); //
}
void allOFF_SN75518() { 
  digitalWrite(VFD_stb1, HIGH); //

  delayMicroseconds(1);
  
  send_char_without(0B00000000);  send_char_without(0B00000000); //   1:8   -   9:16//
  send_char_without(0B00000000);  send_char_without(0B00000000); //  17:24  -  25:32//
  //second set of 75518 32 outputs
  delayMicroseconds(1);
  send_char_without(0B00000000);  send_char_without(0B00000000); //  33:40  -  41:48//
  send_char_without(0B00000000);  send_char_without(0B00000000); //  49:56  -  57:46//

  digitalWrite(VFD_latchBoth, HIGH); //
  delayMicroseconds(1);
  digitalWrite(VFD_latchBoth, LOW); //
  delayMicroseconds(1);
  digitalWrite(VFD_stb1, LOW); //
}
void testClock(){
//................66666555555555544444444443333333//..33322222222221111111111000000000
//................43210987654321098765432109876543//..21098765432109876543210987654321
long arr[120] ={0b00000000000000000000000000000000, 0b00100000000000000000000000000000,//0 
                0b00000000000000000000000000000000, 0b10000000000000000000000000000000,//2
                0b00000000000000000000000000000000, 0b00000000000000100000000000000000,//4
                0b00000000000000000000000000000000, 0b00000000000010000000000000000000,//6
                0b00000000000000000000000000000000, 0b00000000100000000000000000000000,//8
                0b00000000000000000000000000000000, 0b00000000001000000000000000000000,//10
                0b00000000000000000000000000000000, 0b00000000010000000000000000000000,//12
                0b00000000000000000000000000000000, 0b00000000000100000000000000000000,//14
                0b00000000000000000000000000000000, 0b00000000000001000000000000000000,//16
                0b00000000000000000000000000000000, 0b00000000000000010000000000000000,//18      
                //
                0b00000000000000000000000000000000, 0b01000000000000000000000000000000,//20 
                0b00000000000000000000000000000000, 0b00001000000000000000000000000000,//22   
                0b00000000100000000000000000000000, 0b00000000000000000000000000000000,//24
                0b00000000001000000000000000000000, 0b00000000000000000000000000000000,//26
                0b00000000000010000000000000000000, 0b00000000000000000000000000000000,//28
                0b00000000000000100000000000000000, 0b00000000000000000000000000000000,//30
                0b10000000000000000000000000000000, 0b00000000000000000000000000000000,//32
                0b00100000000000000000000000000000, 0b00000000000000000000000000000000,//34
                0b00001000000000000000000000000000, 0b00000000000000000000000000000000,//36
                0b00000001000000000000000000000000, 0b00000000000000000000000000000000,//38 
                //
//................66666555555555544444444443333333//..33322222222221111111111000000000
//................43210987654321098765432109876543//..21098765432109876543210987654321
                0b00000000000000000000000001000000, 0b00000000000000000000000000000000,//40 
                0b00000000000000000000000000010000, 0b00000000000000000000000000000000,//42
                0b00000000000000000000000000001000, 0b00000000000000000000000000000000,//44
                0b00000000000000000000000000000010, 0b00000000000000000000000000000000,//46
                0b00000000000000001000000000000000, 0b00000000000000000000000000000000,//48
                0b00000000000000000100000000000000, 0b00000000000000000000000000000000,//50 
                0b00000000000000000001000000000000, 0b00000000000000000000000000000000,//52
                0b00000000000000000000010000000000, 0b00000000000000000000000000000000,//54
                0b00000000000000000000000100000000, 0b00000000000000000000000000000000,//56
                0b00000000000000000000000000000000, 0b00000000000000000000000001000000,//58
                //
                0b00000000000000000000000000000000, 0b00000000000000000000000000010000,//60 
                0b00000000000000000000000000000000, 0b00000000000000000000010000000000,//62
                0b00000000000000000000000000000000, 0b00000000000000000001000000000000,//64
                0b00000000000000000000000000000000, 0b00000000000000000100000000000000,//66
                0b00000000000000000000000000000000, 0b00000000000000000000000000000001,//68
                0b00000000000000000000000000000000, 0b00000000000000000000000000000100,//70
                0b00000000000000000000000000000000, 0b00000000000000000000000000000010,//72 
                0b00000000000000000000000000000000, 0b00000000000000001000000000000000,//74
                0b00000000000000000000000000000000, 0b00000000000000000010000000000000,//76
                0b00000000000000000000000000000000, 0b00000000000000000000100000000000,//78
                //
//................66666555555555544444444443333333//..33322222222221111111111000000000
//................43210987654321098765432109876543//..21098765432109876543210987654321
                0b00000000000000000000000000000000, 0b00000000000000000000001000000000,//80 
                0b00000000000000000000000000000000, 0b00000000000000000000000000001000,//82
                0b00000000000000000000000000000000, 0b00000000000000000000000000100000,//84
                0b00000000000000000000000000000000, 0b00000000000000000000000010000000,//86
                0b00000000000000000000001000000000, 0b00000000000000000000000000000000,//88 
                0b00000000000000000000100000000000, 0b00000000000000000000000000000000,//90
                0b00000000000000000010000000000000, 0b00000000000000000000000000000000,//92
                0b00000000000000000000000000000001, 0b00000000000000000000000000000000,//94 
                0b00000000000000000000000000000100, 0b00000000000000000000000000000000,//96
                0b00000000000000000000000000100000, 0b00000000000000000000000000000000,//98
                //
                0b00000000000000000000000010000000, 0b00000000000000000000000000000000,//100
                0b00000010000000000000000000000000, 0b00000000000000000000000000000000,//102
                0b00000100000000000000000000000000, 0b00000000000000000000000000000000,//104
                0b00010000000000000000000000000000, 0b00000000000000000000000000000000,//106
                0b01000000000000000000000000000000, 0b00000000000000000000000000000000,//108
                0b00000000000000010000000000000000, 0b00000000000000000000000000000000,//110
                0b00000000000001000000000000000000, 0b00000000000000000000000000000000,//112
                0b00000000000100000000000000000000, 0b00000000000000000000000000000000,//114
                0b00000000010000000000000000000000, 0b00000000000000000000000000000000,//116
                0b00000000000000000000000000000000, 0b00010000000000000000000000000000 //118
};
//This cycle for skip 2 by 2 to control the pointer of minuts. This is only a sample possible.
for (uint8_t s = 0; s < 120; s=s+2){
  if(s >= 120){ //At this point you can put the value you need it stop to debug, between: 0~120.
      while(!flag){
        delay(100);
      }
  }   
    flag = false;
    word7 = (arr[s] & 0xff000000UL) >> 24;
    word6 = (arr[s] & 0x00ff0000UL) >> 16;
    word5 = (arr[s] & 0x0000ff00UL) >>  8;
    word4 = (arr[s] & 0x000000ffUL) >>  0;
    
    word3 = (arr[s+1] & 0xf8000000UL) >> 24;  //Because the 3 bits are assigned to GRID's
    word2 = (arr[s+1] & 0x00ff0000UL) >> 16;
    word1 = (arr[s+1] & 0x0000ff00UL) >>  8;
    word0 = (arr[s+1] & 0x000000ffUL) >>  0;
    animation_clock(word0,word1,word2,word3,word4,word5,word6,word7);
    delay(100);
    Serial.print("s= ");
    Serial.println(s, DEC);
    }
//This cycle for skip 10 by 10 to control pointer of hours clock. This is only a sample possible.    
for (uint8_t s = 0; s < 120; s=s+10){
  if(s >= 120){ //At this point you can put the value you need it stop to debug, between: 0~120.
      while(!flag){
        delay(100);
      }
  }   
    flag = false;
    word7 = (arr[s] & 0xff000000UL) >> 24;
    word6 = (arr[s] & 0x00ff0000UL) >> 16;
    word5 = (arr[s] & 0x0000ff00UL) >>  8;
    word4 = (arr[s] & 0x000000ffUL) >>  0;
    
    word3 = (arr[s+1] & 0xf8000000UL) >> 24;  //Because the 3 bits are assigned to GRID's
    word2 = (arr[s+1] & 0x00ff0000UL) >> 16;
    word1 = (arr[s+1] & 0x0000ff00UL) >>  8;
    word0 = (arr[s+1] & 0x000000ffUL) >>  0;
    animation_clock(word0,word1,word2,word3,word4,word5,word6,word7);
    delay(500);
    Serial.print("s= ");
    Serial.println(s, DEC);
    }
}
void send64bits(){
unsigned long lng32 =  0b00000000000000000000000000000001; // Size of long 4294967295;
unsigned long lng32b = 0b00000000000000000000000000000001;
unsigned int cnt = 0x00;
for (uint8_t n = 0; n<2; n++){
      for (int i = 0; i<32; i++){
        while(!flag){  
                delay(500);
              }
              flag = false;
            switch (n){
              case 0:{
                lng32 = (lng32 << 1);
                word3 = (lng32 & 0xf8000000UL) >> 24;  //Because the 3 bits are assigned to GRID's
                word2 = (lng32 & 0x00ff0000UL) >> 16;
                word1 = (lng32 & 0x0000ff00UL) >>  8;
                word0 = (lng32 & 0x000000ffUL) >>  0;
                break;
              }
              case 1:{              
                lng32b = (lng32b << 1);
                Serial.println(lng32b, BIN);
                word7 = (lng32b & 0xff000000UL) >> 24;
                word6 = (lng32b & 0x00ff0000UL) >> 16;
                word5 = (lng32b & 0x0000ff00UL) >>  8;
                word4 = (lng32b & 0x000000ffUL) >>  0;
                break;
             }   
            }
        animation_clock(word0,word1,word2,word3,word4,word5,word6,word7);
        Serial.print(cnt, DEC); Serial.print(" : ");
        Serial.print(word0, BIN);Serial.print(" ; "); Serial.print(word1, BIN);Serial.print(" ; "); Serial.print(word2, BIN);Serial.print(" ; "); Serial.print(word3, BIN);
        Serial.print(" ; ");
        Serial.print(word4, BIN);Serial.print(" ; "); Serial.print(word5, BIN);Serial.print(" ; "); Serial.print(word6, BIN);Serial.print(" ; "); Serial.println(word7, BIN);  
        cnt++;
        delay(500);
      }
    }
}
void setup() {
  pinMode(VFD_clk, OUTPUT);
  pinMode(VFD_in, OUTPUT);
  pinMode(VFD_stb1, OUTPUT);
  pinMode(VFD_stb2, OUTPUT);
  pinMode(VFD_latchBoth, OUTPUT);

  pinMode(ledPin, OUTPUT); // sets the digital pin as output
  Serial.begin(115200); // only to debug

//Comment this next two lines if are not using the butoon and interrupts, to avoid fail at running because ausence of button at button 2 & 3 pins! 
  attachInterrupt(digitalPinToInterrupt(2),buttonPressed1,FALLING);  // Configuration of Interrupt to the pin 2
  //attachInterrupt(digitalPinToInterrupt(3),buttonPressed2,RISING);  // Configuration of Interrupt to the pin 3
  sei();

}
void loop() {
        allON_SN75518_withoutSymbols();
        delay(1000); // waits for a second
        allON_SN75518_clock();
        delay(2000); // waits for two second
        
        digitalWrite(ledPin, LOW); // sets the LED off
        allOFF_SN75518(); // All off
        //  
       digitalWrite(ledPin, HIGH); // sets the LED on
        allON_SN75518();  // All on
        delay(2000); // waits for two second
        //
        allON_SN75518_grid_1();
        delay(1000); // waits for two second
        allON_SN75518_grid_2();
        delay(1000); // waits for two second
        allON_SN75518_grid_3();
        delay(1000); // waits for two second
        
        msgHELLO_grid_1();
        delay(1000); // waits for two second
        allOFF_SN75518(); // All off
        //
              for (unsigned int v=0; v < 50; v++){  //Used like dinamic VFD control.
              msgPEOPLE_grid_1();
              delay(10); 
              msgPEOPLE_grid_2();
              delay(10); 
              }
//   //send64bits();  //Used to debug, if is not at use comment all lines related to interrupt and trigger events!
    testClock();
    delay(1000);
}
/*****************************************  ISR *************************************/
void buttonPressed1() {
  // knob1();  // Call this function
  flag = true;
  //Serial.println("Reach this point!");  //Comment if isn't in use!
}

void buttonPressed2() {
  //knob2(); // Call this function
}
