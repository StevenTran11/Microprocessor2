const int red1Pin = 3; //red1 led attach to
const int yellow1Pin = 26; //yellow1 led attach to
const int green1Pin = 28; //green1 led attach to
const int button = 2;//switch attach to
bool press = false;
int buzzer = 6;
int CA_1 = 7;
int CA_2 = 8;
int CA_3 = 9;
int CA_4 = 10;
int com = 2;
const int STcp = 12;//Pin connected to ST_CP of 74HC595
const int SHcp = 11;//Pin connected to SH_CP of 74HC595
const int DS = 13; //Pin connected to DS of 74HC595
int count = 0;
int first_digit = 0;
int second_digit = 0;
//display 1,2,3,4,5,6,7,8,9
byte datArray[10] {B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110};

void setup()
{
  pinMode(com, OUTPUT);
  pinMode(red1Pin, OUTPUT);  //set the redPin as an output
  pinMode(yellow1Pin, OUTPUT);  //set the yellowPin as an output
  pinMode(green1Pin, OUTPUT);  //set the greenPin as an output
  pinMode(button, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  //set pins to output
  pinMode(STcp, OUTPUT);
  pinMode(SHcp, OUTPUT);
  pinMode(DS, OUTPUT);
  pinMode(CA_1, OUTPUT);
  pinMode(CA_2, OUTPUT);
  pinMode(CA_3, OUTPUT);
  pinMode(CA_4, OUTPUT);
  digitalWrite(com, HIGH);
  digitalWrite(CA_1, HIGH);
  digitalWrite(CA_2, HIGH);
  digitalWrite(CA_3, HIGH);
  digitalWrite(CA_4, HIGH);
  
  Serial.begin(9600);  // start serial port at 9600 bps:
}

void loop()
{
  if (digitalRead(button) == LOW) //check button press
  {
    press = true;
  }
  if (press == false) //red blink
  {
    digitalWrite(red1Pin, HIGH);
    delay(1000);// turn the LED on
    digitalWrite(red1Pin, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);

  }
  if (press == true)
  {
    while (true)
    {
      for (byte num = 20; num > 2; num--) //display 9-4 and turn on a green led
      {
         
         digitalWrite(red1Pin, HIGH);
         twodigit(num);
         
         
      }

      for (int num = 2; num >= 0; num--) //display 3-1 and beep
      {



        digitalWrite(red1Pin, HIGH);
        twodigit_wbeep(num);


      }
      digitalWrite(red1Pin, LOW);  //turn off
      for (int num = 15 ; num > 2; num--)
      {


        digitalWrite(green1Pin, HIGH);
        twodigit(num);
      }
      for (int num = 2; num >= 0; num--)
      {



        digitalWrite(green1Pin, HIGH);
        twodigit_wbeep(num);
        

      }
      digitalWrite(green1Pin, LOW);

      for (int num = 6 ; num > 2; num--)
      {
        digitalWrite(yellow1Pin, HIGH);
        twodigit(num);
      }

      for (int num = 2 ; num >= 0; num--)
      {
        
        twodigit_wbeepflash(num);
      }
      

      digitalWrite(yellow1Pin, LOW);
    }
  }
}



void segwrite(byte number){
  digitalWrite(STcp, LOW);
  shiftOut(DS, SHcp, LSBFIRST, datArray[number]);
  digitalWrite(STcp, HIGH);
}
void onedigit(int which, byte value){
  digitalWrite(which, LOW);
  segwrite(value);
  delay(10);
  digitalWrite(which,HIGH);
}
void twodigit(byte value){
  int digit0=value/10;
  int digit1=value-(digit0*10);
  for(int i=0;i<50;i++){
    onedigit(CA_1,digit0);
    onedigit(CA_2,digit1);}
 
}
void onedigitloop(int which){
  for (byte digit=0;digit<10;digit++){
    onedigit(which,digit);
  }
}
void onedigit_wbeep(int which, int value){
  digitalWrite(which, LOW);
  segwrite(value);
  digitalWrite(buzzer, HIGH);
  delay(1);
  digitalWrite(buzzer, LOW);
  delay(1);
  delay(8);
  digitalWrite(which,HIGH);
}  
void twodigit_wbeep(int value){
  int digit0=value/10;
  int digit1=value-(digit0 * 10);
  for(int i=0;i<50;i++){
    onedigit_wbeep(CA_1,digit0);
    onedigit_wbeep(CA_2,digit1);}
 
}
void onedigit_wbeepflash(int which, int value){
  digitalWrite(which, LOW);
  segwrite(value);
  
  digitalWrite(yellow1Pin, HIGH);
  digitalWrite(buzzer, HIGH);
  delay(1);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(yellow1Pin, LOW);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
  delay(100);
  digitalWrite(which,HIGH);
}  
void twodigit_wbeepflash(int value){
  int digit0=value/10;
  int digit1=value-(digit0 * 10);
  
    onedigit_wbeepflash(CA_1,digit0);
    onedigit_wbeepflash(CA_2,digit1);
 
}
