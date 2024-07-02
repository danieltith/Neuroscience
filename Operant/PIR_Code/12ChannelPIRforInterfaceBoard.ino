//This sketch was adapted from the 6 channel code in the COMPASS paper.
//Data is sent every second across a serial (USB) connection, as a comma-separated string of values.
// The sketch is written longhand, in an attempt to improve clarity.  Use of arrays for values and counters to improve brevity and flexibility:
// http://arduino.cc/en/Reference/For  for details on arrays  and
// http://arduino.cc/en/Reference/While  for other timing options


// Please refer to schematic and solder jumpers to identify which channel Light dependent resistors (LDRs) are reading from

String ID = "D";  // data header
int LDRPin1 = A0;     //pin definition for LDR1
int LDRPin2 = A1;     //pin definition for LDR2
int LDRPin3 = A2;     //pin definition for LDR3
int LDRPin4 = A3;     //pin definition for LDR4
int LDRPin5 = A4;     //pin definition for LDR5
int LDRPin6 = A5;     //pin definition for LDR6
int LDRPin7 = A6;     //pin definition for LDR7
int LDRPin8 = A7;     //pin definition for LDR8

int PIR1 = 2;     //pin definition for PIR1
int PIR2 = 3;     //pin definition for PIR2
int PIR3 = 4;     //pin definition for PIR3
int PIR4 = 5;     //pin definition for PIR4
int PIR5 = 6;     //pin definition for PIR5
int PIR6 = 7;     //pin definition for PIR6
int PIR7 = 8;     //pin definition for PIR7
int PIR8 = 9;     //pin definition for PIR8
int PIR9 = 10;    //pin definition for PIR9
int PIR10 = 11;   //pin definition for PIR10
int PIR11 = 12;   //pin definition for PIR11
int PIR12 = 13;   //pin definition for PIR12

int PIRCounter1 = 0;   //activity counter for PIR1
int PIRCounter2 = 0;   //activity counter for PIR2
int PIRCounter3 = 0;   //activity counter for PIR3
int PIRCounter4 = 0;   //activity counter for PIR4
int PIRCounter5 = 0;   //activity counter for PIR5
int PIRCounter6 = 0;   //activity counter for PIR6
int PIRCounter7 = 0;   //activity counter for PIR7
int PIRCounter8 = 0;   //activity counter for PIR8
int PIRCounter9 = 0;   //activity counter for PIR9
int PIRCounter10 = 0;  //activity counter for PIR10
int PIRCounter11 = 0;  //activity counter for PIR11
int PIRCounter12 = 0;  //activity counter for PIR12

int LoopCounter = 0; //loopcounter for activity

void setup() {

  Serial.begin(57600);
  // give time for startup
  delay(5000);
}

// This section runs in a loop constantly
// sensors are polled every 100msec
// data sent via serial every second
void loop() {

  // read all the input pins:
  int PIRState1 = digitalRead(PIR1);
  int PIRState2 = digitalRead(PIR2);
  int PIRState3 = digitalRead(PIR3);
  int PIRState4 = digitalRead(PIR4);
  int PIRState5 = digitalRead(PIR5);
  int PIRState6 = digitalRead(PIR6);
  int PIRState7 = digitalRead(PIR7);
  int PIRState8 = digitalRead(PIR8);
  int PIRState9 = digitalRead(PIR9);
  int PIRState10 = digitalRead(PIR10);
  int PIRState11 = digitalRead(PIR11);
  int PIRState12 = digitalRead(PIR12);

  if (LoopCounter < 10) { 
    // measurements will keep looping until 1 sec is up (10 x 100 msec)
    // if the state is high, increment the relevent PIRcounter
    if (PIRState1 == HIGH) {
      // e.g. if the current state of PIR1 is HIGH then add 1 to the PIR1 counter:
      PIRCounter1++;
    }
    if (PIRState2 == HIGH) {
      PIRCounter2++;
    }
    if (PIRState3 == HIGH) {
      PIRCounter3++;
    }
    if (PIRState4 == HIGH) {
      PIRCounter4++;
    }
    if (PIRState5 == HIGH) {
      PIRCounter5++;
    }
    if (PIRState6 == HIGH) {
      PIRCounter6++;
    }
    if (PIRState7 == HIGH) {
      PIRCounter7++;
    }
    if (PIRState8 == HIGH) {
      PIRCounter8++;
    }
    if (PIRState9 == HIGH) {
      PIRCounter9++;
    }
    if (PIRState10 == HIGH) {
      PIRCounter10++;
    }
    if (PIRState11 == HIGH) {
      PIRCounter11++;
    }
    if (PIRState12 == HIGH) {
      PIRCounter12++;
    }
    delay(100);  // wait 100msec before polling inputs again.  
    //This timing can be varied to ensure consistent 1 sec updates.
    LoopCounter++;
  }
  else {

    //send HEADER indicating data follows
    Serial.print(ID);
    // separate with a comma
    Serial.print(",PIRs:,");
    // send total activity for PIR counters, separated by commas
    Serial.print(PIRCounter1);
    Serial.print(",");
    Serial.print(PIRCounter2);
    Serial.print(",");
    Serial.print(PIRCounter3);
    Serial.print(",");
    Serial.print(PIRCounter4);
    Serial.print(",");
    Serial.print(PIRCounter5);
    Serial.print(",");
    Serial.print(PIRCounter6);
    Serial.print(",");
    Serial.print(PIRCounter7);
    Serial.print(",");
    Serial.print(PIRCounter8);
    Serial.print(",");
    Serial.print(PIRCounter9);
    Serial.print(",");
    Serial.print(PIRCounter10);
    Serial.print(",");
    Serial.print(PIRCounter11);
    Serial.print(",");
    Serial.print(PIRCounter12);
    Serial.print(",LDRs:,");
    // read LDRs connected to relevant pins and send resulting number
    Serial.print(analogRead(LDRPin1));
    Serial.print(",");
    Serial.print(analogRead(LDRPin2));
    Serial.print(",");
    Serial.print(analogRead(LDRPin3));
    Serial.print(",");
    Serial.print(analogRead(LDRPin4));
    Serial.print(",");
    Serial.print(analogRead(LDRPin5));
    Serial.print(",");
    Serial.print(analogRead(LDRPin6));
    Serial.print(",");
    Serial.print(analogRead(LDRPin7));
    Serial.print(",");
    Serial.print(analogRead(LDRPin8));
    Serial.print(",");
    Serial.print('\n'); // new line (linefeed) character
    LoopCounter = 0; // reset your loops
    PIRCounter1 = 0; // and your individual counters
    PIRCounter2 = 0;
    PIRCounter3 = 0;
    PIRCounter4 = 0;
    PIRCounter5 = 0;
    PIRCounter6 = 0;
    PIRCounter7 = 0;
    PIRCounter8 = 0;
    PIRCounter9 = 0;
    PIRCounter10 = 0;
    PIRCounter11 = 0;
    PIRCounter12 = 0;
  }
}
