#define led 13  // built-in LED
float ByteReceived;
float OldP = 0;         // Previous value used to calculate change in P //  DELTA P
float P = 0;            //  Proportional component
float I = 0;            //  Integral        just the sum of P over time
float OldI = 0;         //  previous value of I for calculation of Delta I
float D = 0;            //  Differential       D = P - OldP
float bp = -60;         // balance point
float pwm = 0;          // value of Pulse Width Modulation  to ENA ENB
long a = 0;             // L298N to IN 1 to 4
long b = 0;             //
const int PinR1 = 5;    //  arduino  pin 5 to l298  pin IN4
const int PinR2 = 6;    //  arduino  pin 6 to l298  pin IN3
const int PinL1 = 7;    //  arduino  pin 7 to l298  pin IN1 
const int PinL2 = 8;    //  arduino  pin 8 to l298  pin IN2
const int PwmR  = 9;    //  arduino  pin 9 to l298  pin ENB
const int PwmL  = 10;   //  arduino  pin 10 to l298  pin ENA
String inputString = "";
boolean stringComplete = false;
static int counter = 0;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
//  Serial.print("Tick #");
//  Serial.print(counter++, DEC);
//  Serial.print("\n");
//  delay(1000);
   if (Serial.available() > 0)
  {

//      Serial.println(inputString.toFloat());
//      Serial.println(inputString.toFloat());
//      Serial.print("\n");
//    Serial.println();    // End the line


      inputString = Serial.readStringUntil('\n');
      ByteReceived = inputString.toFloat();

      if(inputString == "1")
    {
      digitalWrite(LED_BUILTIN,HIGH);
    }

    if(inputString =="0")
    {
      digitalWrite(LED_BUILTIN,LOW);
    }

    
          
          OldP =P;                     // save value of P
          P = (ByteReceived * 1000) + bp;    // update P from MPU add bp to correct for balance
          OldI = I;                    // save old I
          I = I + (P * 0.05) ;
          I = I + ((I - OldI)*2  );     // calulate new I
          if( I >  250 ) I =  250;           // LIMIT  Stop I building up too high
          if( I < -250 ) I = -250;           // or too low value
          D = P - OldP;                      //  D differential   change in P
          pwm = ( P * 1 ) + ( I  ) + ( D * 10 ) ; // P I D

//          Serial.println(pwm); 
          a = 0;
          b = 0;
          if(pwm < 0){
            a = 0;
            b = 1;
             bp = bp - 0.01;
            digitalWrite(13, 0);
          }
          if(pwm > 0){
            a = 1;
            b = 0;
            bp = bp + 0.01;
            digitalWrite(13, 1);
          }

          if(pwm < 0){
            digitalWrite(13, 1);
          }
          if(pwm > 0){
            digitalWrite(13, 0);
          }
          /////////////////////////////
          // remove sign from PWM as - value has no meaning
          pwm  = abs(pwm);
          if ( pwm < 0) pwm = 0;
          if ( pwm > 255) pwm = 255;

            if(abs(ByteReceived) < abs(1.1)){
              analogWrite(PwmR, pwm);
              digitalWrite(PinR1, a);
              digitalWrite(PinR2 ,b);

              analogWrite(PwmL ,pwm);
              digitalWrite(PinL1 ,a);
              digitalWrite(PinL2 ,b);
              }
           else{
              analogWrite(PwmR , 0);
              analogWrite(PwmL , 0);
              I = 0;
              bp = -98;
              delay(1000);
           }
           inputString = "";
           stringComplete = false;
  }           
}



