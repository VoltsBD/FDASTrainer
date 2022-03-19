/*  This program is intended as a temporary FDAS Controller
 *  It can be used on 4 wire Sensors and Call Points Only
 *  Please don't use for 2 Wire Sensors as it doesn't use current detecting sensors only Analog Input for the Continuity and Short Circuit for Triggering.
 *  
 *  Console   5v Supply
 *             |
 *             >
 *            <   R1 Limiting Resistor
 *             >
 *            <
 *             |  Sensing Line
 *  A0--------------------------o----------------------o------------------o
 *                              |                      |                  |
 *                              o4/C2                  o                 <
 *                           Sensor               Call Point              > Resistor
 *                              o3/L2                  o                 <
 *                              |                      |                  |
 * GND--------------------------o----------------------o------------------o
 * 
 * 24v ------|/| ---------------o
 *           R1                 |     
 *                              o1/L1 
 *                           Sensor   
 *                              o2/C2 
 *                              |     
 * GND--------------------------o
 * 
 * 11----0 R1 Coil 0------ GND
 * 
 * 
 * Program and Circuits Created by V.B. Dupo
 * Intended for Educational Purposes Only strictly not for commercial use.
 * This code is published under GITHUB 
 * 
 *  
 */

int valueAO, pvalueAO;
bool test, stp, ringer, conterr, fire;

void setup() {
pinMode(2,INPUT_PULLUP); // Test
pinMode(3,INPUT_PULLUP); // Clear Ringer Button
pinMode(4,OUTPUT);       // Zone 1 Ready
pinMode(5,OUTPUT);      // Zone 1 Continuity Error (Fault)
pinMode(6,OUTPUT);      // Zone 1 Triggered
pinMode(13,OUTPUT);     // SND 1 Lamp
pinMode(10,OUTPUT);     //  SND 1 Relay
pinMode(7,OUTPUT);      // Console Ready
pinMode(8,OUTPUT);      // Console Test
pinMode(9,OUTPUT);      // Buzzer
pinMode(11,OUTPUT);     // Z1 Relay Control
attachInterrupt(digitalPinToInterrupt(2), STRTING, FALLING);
attachInterrupt(digitalPinToInterrupt(3), RINGCLEAR, FALLING);
test=HIGH; // Test Button Not Pressed
stp=HIGH; // Clear Ringer Not Pressed
ringer=LOW; // Ringer Off
conterr=LOW; // No Continuity Error
fire=LOW; // No Sensor or Call Point Activated
pvalueAO=68;
digitalWrite(4,HIGH); // Zone 1 Not Ready
digitalWrite(5,LOW); // Zone 1 No Continuity Error
digitalWrite(6,LOW); // Zone 1 Triggered
digitalWrite(7,HIGH); // Console Ready
digitalWrite(8,LOW); // Console Under Test
digitalWrite(9,LOW); // Buzzer On
digitalWrite(13,LOW); // Sounder Lamp Lit
digitalWrite(10,LOW); // Sounder Relay Closed..
digitalWrite(11,LOW); // 
Serial.begin(9600);
}

void RINGCLEAR() 
{
  digitalWrite(10,LOW);
  digitalWrite(9,LOW);
  delay(10000);        // Stop Beeping for 10 Seconds;
}

void STRTING()
{
if(test==LOW){ test=HIGH; stp=LOW; }
Serial.println("Interrupt Enabled");
delay(1000);
}

void linemonitor()
{
valueAO=analogRead(A0);
if(pvalueAO>10)
{ if(valueAO<1023 && valueAO>5) { conterr=LOW; fire=LOW;  /* system normal */ }
if(valueAO==1023) { conterr=HIGH; fire=LOW; /* there is no fire but system is at fault */  }
 if(valueAO<10) { conterr=LOW; fire=HIGH; /* there is a fire */ }
pvalueAO=valueAO;
}
}

void actions()
{
  if(test==HIGH && stp==LOW && fire==LOW && conterr == LOW)  // Zone 1 Under test but system is normal
  { 
  digitalWrite(4,LOW); // Zone 1 Not Ready
  digitalWrite(5,LOW); // Zone 1 No Continuity Error
  digitalWrite(6,HIGH); // Zone 1 Triggered
  digitalWrite(7,HIGH); // Console Ready
  digitalWrite(8,HIGH); // Console Under Test
  digitalWrite(9,HIGH); // Buzzer On
  digitalWrite(13,HIGH); // Sounder Lamp Lit
  digitalWrite(10,HIGH); // Sounder Relay Closed..
  delay(10000);
  digitalWrite(4,HIGH); // Zone 1 Not Ready
  digitalWrite(5,LOW); // Zone 1 No Continuity Error
  digitalWrite(6,LOW); // Zone 1 Triggered
  digitalWrite(7,HIGH); // Console Ready
  digitalWrite(8,LOW); // Console Under Test
  digitalWrite(9,LOW); // Buzzer On
  digitalWrite(13,LOW); // Sounder Lamp Lit
  digitalWrite(10,LOW); // Sounder Relay Closed..
  test=HIGH; 
  stp=HIGH;   // Reset values to end test after 2 seconds
  }
  if(test==HIGH&& stp==HIGH && fire==LOW && conterr == LOW) // No Continuity Error or Fire Detected
  { 
  digitalWrite(4,HIGH); // Zone 1 Ready
  digitalWrite(5,LOW); // Zone 1 No Continuity Error
  digitalWrite(6,LOW); // Zone 1 Not Triggered
  digitalWrite(7,HIGH); // Console Ready
  digitalWrite(8,LOW); // Console Under Test
  digitalWrite(9,LOW); // Buzzer Off
  digitalWrite(13,LOW); // Sounder Lamp Unlit
  digitalWrite(10,LOW); // Sounder Relay Open
  }
  if(test==HIGH && stp==HIGH && fire==LOW && conterr == HIGH) 
  { 
  digitalWrite(4,LOW); // Zone 1 Not Ready
  digitalWrite(5,HIGH); // Zone 1 No Continuity Error
  digitalWrite(6,LOW); // Zone 1 Triggered
  digitalWrite(7,HIGH); // Console Ready
  digitalWrite(8,LOW); // Console Under Test
  digitalWrite(9,HIGH); // Buzzer On
  digitalWrite(13,LOW); // Sounder Lamp Lit
  digitalWrite(10,LOW); // Sounder Relay Closed..
  }
  if(test==HIGH && stp==HIGH && fire==HIGH && conterr == LOW) 
  { 
  digitalWrite(4,LOW); // Zone 1 Not Ready
  digitalWrite(5,LOW); // Zone 1 No Continuity Error
  digitalWrite(6,HIGH); // Zone 1 Triggered
  digitalWrite(7,HIGH); // Console Ready
  digitalWrite(8,LOW); // Console Under Test
  digitalWrite(9,HIGH); // Buzzer On
  digitalWrite(13,HIGH); // Sounder Lamp Lit
  digitalWrite(10,HIGH); // Sounder Relay Closed..
  digitalWrite(11,HIGH); // Open 24v Supply Relay to Z1
  delay(3000);
  digitalWrite(11,LOW); // Close 24v Supply Relay to Z1
  }
}


void loop() 
{
  linemonitor();
  actions();
  Serial.print("AO Ouput");
  Serial.print(valueAO);
  Serial.print("Test=");
  Serial.print(test);
  Serial.print(" STP=");
  Serial.print(stp);
  Serial.print(" FIRE=");
  Serial.print(fire);
  Serial.print(" Continuity Error=");
  Serial.println(conterr);
}
