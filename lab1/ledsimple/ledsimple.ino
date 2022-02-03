bool press = false;
int red = 7;
int yellow = 5;
int green = 3;
int button = 2;
int buzzer = 28;
int timer;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
}
void secondDelay()
{
  while(timer > 0)//countdown
  {
    delay(1000);
    timer = timer - 1;
    if(timer < 4)//buzzer last 3 seconds
    {
      digitalWrite(buzzer, HIGH);
    }
  }
}
void loop() {
  if(digitalRead(button) == LOW)//if button is not pressed
  {
    press = true;
  }
  if(press == false) //if button not pressed red blinks
  {
    if(digitalRead(red) == 0) //flash 1 second delay
    {
      digitalWrite(red, HIGH);
    }
    else
    {
      digitalWrite(red, LOW);
    }
    secondDelay();
  }
  else //button is pressed
  {
    digitalWrite(red, HIGH);//red on
    timer = 15;
    secondDelay();
    digitalWrite(buzzer, LOW);
    digitalWrite(red, LOW);//red off
    digitalWrite(green, HIGH);//green on
    timer = 15;
    secondDelay();
    digitalWrite(buzzer, LOW);
    digitalWrite(green, LOW);//green off
    digitalWrite(yellow, HIGH);//yellow1 on
    timer = 2;
    secondDelay();
    timer = 10;
    while(timer > 0)
    {
      if(digitalRead(yellow) == 0) //flash 0.1 second delay
      {
        digitalWrite(yellow, HIGH)
      }
      else
      {
        digitalWrite(yellow, LOW);
      }
      delay(100);
      timer = timer - 1;
    }
    digitalWrite(buzzer, LOW);
  }
}
