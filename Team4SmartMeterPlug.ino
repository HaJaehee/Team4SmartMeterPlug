#include <LiquidCrystal.h>

/*
  modified 11 May 2015
  by HJH
 */
int SSRPin = 13;
int WCSPin = A0;
double A0Value=0;
  
double initSum = 0.0;
double initAvg = 0.0;
double rmsArray[50];
double result;
int N=50;
int initN=200;
int counter=0;
int initCounter=0;

int buttonPin[16] = {22,23,24,25,26,27,28,29};
int ledCols[8] = {30,31,32,33,34,35,36,37};
int ledRows[8] = {14,15,16,17,18,19,20,21};
LiquidCrystal lcd(12,11,5,4,3,2);

bool SSROn = false;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(SSRPin, OUTPUT);
  digitalWrite(SSRPin, LOW);   
  SSROn = false;
  for(int i = 0 ; i < 8 ; i++)
  {
    pinMode(buttonPin[i],INPUT);
    pinMode(ledCols[i],OUTPUT);
    digitalWrite(ledCols[i],HIGH);
    pinMode(ledRows[i],OUTPUT);
    digitalWrite(ledRows[i],LOW);
  }
  lcd.begin(16,2);
  lcd.print("SSR is off.");
  
  setInitA0ValueAvg();
  Serial.begin(9600);
}
void matrixOff (){
  for ( int i = 0 ; i < 8 ; i++ )
  {
    digitalWrite(ledRows[i],LOW);
    digitalWrite(ledCols[i],HIGH);
  }
}
  
void setInitA0ValueAvg()
{
  initSum=0;
  initAvg=0;
    for (initCounter=0;initCounter<initN;initCounter++)
  {
    A0Value=analogRead(WCSPin);
    initSum += A0Value;
  }
  initAvg = initSum/initN;
}
// the loop function runs over and over again forever
void loop() {

  
/*
  for( int i = 0 ; i < 8 ; i++)
  {
    digitalWrite(ledCols[i],LOW);
    for( int j = 0 ; j < 8 ; j++)
    {
      digitalWrite(ledRows[j],HIGH);
    }
    delay(1000);
  }
  matrixOff();
  for (int i = 0 ; i < 8 ; i++ )
  {
    digitalWrite(ledRows[i],HIGH);
    for(int j = 0 ; j < 8 ; j++)
    {
      digitalWrite(ledCols[j],LOW);
    }
    delay(1000);
  }
  matrixOff();*/
  if (SSROn)
  {
    digitalWrite(ledRows[0],HIGH);
    digitalWrite(ledCols[1],LOW);
    digitalWrite(ledCols[2],LOW);
    digitalWrite(ledCols[4],LOW);
    digitalWrite(ledCols[7],LOW);
    delay(5);
    matrixOff();
    digitalWrite(ledRows[1],HIGH);
    digitalWrite(ledCols[0],LOW);
    digitalWrite(ledCols[3],LOW);
    digitalWrite(ledCols[4],LOW);
    digitalWrite(ledCols[5],LOW);
    digitalWrite(ledCols[7],LOW);
    delay(5);
    matrixOff();
    digitalWrite(ledRows[2],HIGH);
    digitalWrite(ledCols[0],LOW);
    digitalWrite(ledCols[3],LOW);
    digitalWrite(ledCols[4],LOW);
    digitalWrite(ledCols[6],LOW);
    digitalWrite(ledCols[7],LOW);
    delay(5);
    matrixOff();
    digitalWrite(ledRows[3],HIGH);
    digitalWrite(ledCols[1],LOW);
    digitalWrite(ledCols[2],LOW);
    digitalWrite(ledCols[4],LOW);
    digitalWrite(ledCols[7],LOW);
    delay(5);
    matrixOff();
  }
  else if (!SSROn)
  {
    digitalWrite(ledRows[0],HIGH);
    digitalWrite(ledCols[1],LOW);
    digitalWrite(ledCols[2],LOW);
    digitalWrite(ledCols[4],LOW);
    digitalWrite(ledCols[5],LOW);
    digitalWrite(ledCols[6],LOW);
    digitalWrite(ledCols[7],LOW);
    delay(5);
    matrixOff();
    digitalWrite(ledRows[1],HIGH);
    digitalWrite(ledCols[0],LOW);
    digitalWrite(ledCols[3],LOW);
    digitalWrite(ledCols[4],LOW);
    delay(5);
    matrixOff();
    digitalWrite(ledRows[2],HIGH);
    digitalWrite(ledCols[0],LOW);
    digitalWrite(ledCols[3],LOW);
    digitalWrite(ledCols[4],LOW);
    digitalWrite(ledCols[5],LOW);
    digitalWrite(ledCols[6],LOW);
    digitalWrite(ledCols[7],LOW);
    delay(5);
    matrixOff();
    digitalWrite(ledRows[3],HIGH);
    digitalWrite(ledCols[1],LOW);
    digitalWrite(ledCols[2],LOW);
    digitalWrite(ledCols[4],LOW);
    delay(5);
    matrixOff();
    
  }
  
  if (!digitalRead(buttonPin[0]) && !SSROn)
  {
    digitalWrite(SSRPin, HIGH);
    SSROn = true;
    lcd.setCursor(0,0);
    lcd.print("SSR is on. ");
    Serial.println("======SSR ON======");
  }
  
  if(!digitalRead(buttonPin[1]) && SSROn)
  {
    digitalWrite(SSRPin, LOW);
    SSROn = false;
    lcd.setCursor(0,0);
    lcd.print("SSR is off.");
    Serial.println("======SSR OFF======");
    setInitA0ValueAvg();
  }
  
   
  //digitalWrite(SSRPin, HIGH);   // turn the LED on (HIGH is the voltage level)

  
  if (counter<N){
    A0Value=analogRead(WCSPin);
    rmsArray[counter] = A0Value- initAvg;
    counter++;
  }
  else {
    counter=0;
    result=0;
    for(int i = 0; i<N ; i++)
    {
      result += pow(rmsArray[i],2);
    }
    result /= N;
    result = sqrt(result);
    Serial.print("A0 = ");
    Serial.println(result);
  }
  //digitalWrite(SSRPin, LOW);    // turn the LED off by making the voltage LOW
  //Serial.println("======LED OFF======");
  
}
