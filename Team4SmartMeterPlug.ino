#include <LiquidCrystal.h>

/*
  modified 11 May 2015
  by HJH
 */
int SSRPin = 13;
int WCSPin = A0;
double A0Value=0;
  
double initSensInputSum = 0.0;
double initSensInputAvg = 0.0;
double valueForCorrectionError = 0;
double rmsArray[200];
double result;
int sensLoopN=200;
int initLoopN=200;
int counter=0;
int initSensInputCounter=0;
int ssrPower=0;

int buttonPin[16] = {22,23,24,25,26,27,28,29};
int ledCols[8] = {30,31,32,33,34,35,36,37};
int ledRows[8] = {14,15,16,17,18,19,20,21};
LiquidCrystal lcd(12,11,5,4,3,2);

bool SSROn = false;
bool initiated = false;

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
  ssrPower=0;
  initiated = true;
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
  
  delay(2000);
  initSensInputSum=0;
  initSensInputAvg=0;
    for (initSensInputCounter=0;initSensInputCounter<initLoopN;initSensInputCounter++)
  {
    A0Value=analogRead(WCSPin);
    initSensInputSum += A0Value;
  }
  initSensInputAvg = initSensInputSum/initLoopN;
}
// the loop function runs over and over again forever
void loop() {

  if (Serial.available())
  {
      Serial.println(Serial.read());
  }
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
    //Serial.println("======SSR ON======");
    ssrPower=1;
  }
  
  if(!digitalRead(buttonPin[1]) && SSROn)
  {
    digitalWrite(SSRPin, LOW);
    SSROn = false;
    lcd.setCursor(0,0);
    lcd.print("SSR is off.");
    //Serial.println("======SSR OFF======");
    setInitA0ValueAvg();
    ssrPower=0;
    initiated=true;
  }
  
   
  //digitalWrite(SSRPin, HIGH);   // turn the LED on (HIGH is the voltage level)

  
  if (counter<sensLoopN){
    A0Value=analogRead(WCSPin);
    rmsArray[counter] = A0Value-initSensInputAvg;
    counter++;
  }
  else {
    counter=0;
    result=0;
    for(int i = 0; i<sensLoopN ; i++)
    {
      result +=  pow(rmsArray[i],2);
    }
    result /= sensLoopN; 
    result = sqrt(result); //rms value
    result = result/4.44416; //rms * 1024/5 * 0.0217. 0.0217 value is from WCS1600 document.
    result = 220*result; //calculate value of watt
    if (initiated) //when the Arduino is on first time, it collects value of watt to correct error
    {
      valueForCorrectionError = result;
      initiated = false;
    }
    else if (!SSROn) //when the SSR is off, it collects values of watt to correct error
    {
      valueForCorrectionError += result;
      valueForCorrectionError /= 2.0;
    } 
    result -=  valueForCorrectionError; //correct error
    if (result <10) //correct error
      result=0;
    
    Serial.print(result);
    Serial.print(",");
    Serial.println(ssrPower);
  }
  //digitalWrite(SSRPin, LOW);    // turn the LED off by making the voltage LOW
  //Serial.println("======LED OFF======");
  
}
