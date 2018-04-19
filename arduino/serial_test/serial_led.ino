#define led 13  // built-in LED
int ByteReceived;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  if (Serial.available() > 0)
  {
    ByteReceived = Serial.read();

    if(ByteReceived == '1')
    {
      digitalWrite(LED_BUILTIN,HIGH);
    }

    if(ByteReceived == '0')
    {
      digitalWrite(LED_BUILTIN,LOW);
    }

    Serial.println();    // End the line

  }
}

