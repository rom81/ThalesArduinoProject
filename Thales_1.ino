/*
  Created by Rachel Misbin
  Thursday, November 17, 2016
  Thales Project Arduino Competition
  Reads sensors (a distance sensor, a force sensor, and three photoresistors)
  and turns on/off red and green LEDs based on analog sensor value readings.
  Designed for use with a train system to prevent people from getting hit
  by trains after falling on the tracks.
*/

const int distancePin = 0;          // distance pin:             analog 0
const int fsrPin = 1;               // force sensitive resistor: analog 1
const int photo1 = 2;               // photoresistor 1 pin:      analog 2
const int photo2 = 3;               // photoresistor 2 pin:      analog 3
const int photo3 = 4;               // photoresistor 3 pin:      analog 4
const int buzzerPin = 4;            // buzzer pin:               digital 4
const int GreenLED = 5;             // green LED:                digital 5
const int RedLED = 6;               // red LED:                  digital 6

boolean distanceAlert = false;      // distance alert  
boolean forceAlert = false;         // force alert
boolean photoAlert_1 = false;       // photoresistor 1 alert
boolean photoAlert_2 = false;       // photoresistor 2 alert
boolean photoAlert_3 = false;       // photoresistor 3 alert
boolean ALERT;

float force;                        // stores force from force sensor
float distance;                     // stores distance from distance sensor
float photoValue_1;                 // stores value from photoresistor 1
float photoValue_2;                 // stores value from photoresistor 2
float photoValue_3;                 // stores value from photoresistor 3

void setup() {
  pinMode(GreenLED, OUTPUT);    
  pinMode(RedLED, OUTPUT);         
  pinMode(buzzerPin, OUTPUT);         
  Serial.begin(9600);             
}

void loop() {

  // 1: Read analog sensor values
  force = analogRead(fsrPin);             
  distance = 12343.85*pow(analogRead(distancePin), -1.15);  // scaled to cm
  photoValue_1 = analogRead(photo1);      
  photoValue_2 = analogRead(photo2);    
  photoValue_3 = analogRead(photo3);  

  // 2: Display sensor values on serial monitor
  Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm");
  Serial.print("Force: "); Serial.println(force); 
  Serial.print("Photoresistor 1: "); Serial.println(photoValue_1);
  Serial.print("Photoresistor 2: "); Serial.println(photoValue_2);
  Serial.print("Photoresistor 3: "); Serial.println(photoValue_3);

  // 3: Calculate alerts
  distanceAlert = checkDistance(distance);            
  forceAlert = checkForce(force);                    
  photoAlert_1 = checkPhotoresistor(photoValue_1);    
  photoAlert_2 = checkPhotoresistor(photoValue_2);   
  photoAlert_3 = checkPhotoresistor(photoValue_3);
  ALERT = distanceAlert && forceAlert && (photoAlert_1 || photoAlert_2 || photoAlert_3);

  // 4: Act on alerts
  while (!distanceAlert) {
    distance = 12343.85*pow(analogRead(distancePin), -1.15);
    distanceAlert = checkDistance(distance); 
    digitalWrite(RedLED, LOW);
    digitalWrite(GreenLED, LOW);
    noTone(buzzerPin);
  }
  if (ALERT) {         
    digitalWrite(RedLED, HIGH);     // RED led is on; green is off
    digitalWrite(GreenLED, LOW); 
    tone(buzzerPin, 440);           // set buzzer tone to 440 Hz
  }
  else {   
    digitalWrite(GreenLED, HIGH);   // GREEN led is on; red is off
    digitalWrite(RedLED, LOW);
    noTone(buzzerPin);              // set buzzer to off
  }
 
}

// returns whether or not the distance is within valid range
boolean checkDistance(float distance) { 
  return distance > 10 ? true : false;  // modify conditions of range
}
// returns whether or not the force is within valid range
boolean checkForce(float force) {
  return force > 10 ? true : false; // modify conditions of range
}
// returns whether or not the brightness is within range
boolean checkPhotoresistor(float brightness) {
  return brightness < 900 ? true : false;  // modify conditions of range
}
