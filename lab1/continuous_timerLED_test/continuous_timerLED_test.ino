#include "Timer.h" 
Timer timer; 

long number = 0; 
int num1 = 0;
int num2 = 0;
int num3 = 0;
int num4 = 0;
int timer_event = 0;
int numbers[4] ;
int count = 0;

#define NUM_OF_DIGITS 4
int latch = 4; //74HC595  pin 9 STCP
int cs = 5; //74HC595  pin 10 SHCP
int data = 3; //74HC595  pin 8 DS
int dPins[4] = {11, 10, 9, 8};
//  DP G F E D C B A
//0: 1 1 0 0 0 0 0 0
//1: 1 1 1 1 1 0 0 1
//2: 1 0 1 0 0 1 0 0
//3: 1 0 1 1 0 0 0 0
//4: 1 0 0 1 1 0 0 1
//5: 1 0 0 1 0 0 1 0
//6: 1 0 0 0 0 0 1 0
//7: 1 1 1 1 1 0 0 0
//8: 1 0 0 0 0 0 0 0
//9: 1 0 0 1 0 0 0 0
//10:1 0 0 0 1 0 0 0
//11:1 0 0 0 0 0 1 1
//12:1 1 0 0 0 1 1 0
//13:1 0 1 0 0 0 0 1
//14:1 0 0 0 0 1 1 0
//15:1 0 0 0 1 1 1 0
byte table[16] {B00000011, B10011111, B00100101, B00001101, B10011001, B01001001, B01000001, B00011111, B00000001, B00001001, B00010001, B11000001, B01100011, B10000101, B01100001, B01110001};

void setup() 
{
  Serial.begin(115200);
  pinMode(latch, OUTPUT);
  pinMode(cs, OUTPUT);
  pinMode(data, OUTPUT);
  for (int j = 0; j < NUM_OF_DIGITS; j++)
  {
    pinMode(dPins[j], OUTPUT);
    pinMode(dPins[j], HIGH);
  }
}
void loop() 
{
  //Count from 0 to 9 on each digit
  /*for (int i = 0; i < NUM_OF_DIGITS; i++) 
  {
    for (int j = 0; j < 16; j++) {
      Display(i, j);
      delay(500);
    }
  }*/
  timer.update();
  if (Serial.available())
  { 
    timer.stop(timer_event); 
    screenOff(); 
    String s = "9999";
    number = (long)s.toInt(); 
    if (number > 9999) 
    { 
      Serial.println("Enter a number between 0 and 9999");
    }
    else 
    {
      separate(number);
      timer_event = timer.every(1, Display);
    }
  }
}

void separate(long num)
{ 
  num1 = num / 1000;
  numbers[0] = num1;
  int num1_remove = num - (num1 * 1000);
  num2 = num1_remove / 100;
  numbers[1] = num2;
  int num2_remove = num1_remove - (num2 * 100);
  num3 = num2_remove / 10;
  numbers[2] = num3;
  num4 = num2_remove - (num3 * 10);
  numbers[3] = num4;
}

void Display(int id, unsigned char num)
{
  /*digitalWrite(latch, LOW);
  shiftOut(data, cs, MSBFIRST, table[num]);
  digitalWrite(latch, HIGH);
  for (int j = 0; j < NUM_OF_DIGITS; j++) 
  {
    digitalWrite(dPins[j], LOW);
    digitalWrite(dPins[id], HIGH);
  }*/
  screenOff(); 
  digitalWrite(latch, LOW); 
  shiftOut(data, cs,LSBFIRST, table[numbers[count]]); 
  digitalWrite(dPins[count], LOW); 
  digitalWrite(latch, HIGH); 
  count++; 
  if (count == 4) { 
    count = 0;
  }
}

void screenOff()
{
  for(int i = 0; i < NUM_OF_DIGITS; i++)
  {
    digitalWrite(dPins[i], HIGH);
  }
}
