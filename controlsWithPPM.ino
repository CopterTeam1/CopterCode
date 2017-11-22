#define numOfChannels 6     // How many radio channels
int channel[numOfChannels]; /* Store read channel values here
                             *   channel[0] -> channel 1 -> roll
                             *   channel[1] -> channel 2 -> pitch
                             *   channel[2] -> channel 3 -> thrust
                             *   channel[3] -> channel 4 -> yaw
                             * We're ignoring the rest of the channels for now
                             */
                            
int PPMInput = 4;

// to catch values from PPM
int thrustVal;
int rollVal;
int yawVal;
int pitchVal;

// to drive motors seperately
int motor1Pin = 3;  // front left
int motor2Pin = 5;  // front right
int motor3Pin = 6;  // back left
int motor4Pin = 9;  // back right

// to access all motors at the same time
int allMotors[] = {3, 5, 6, 9};

// Values to control levels getting output to the motors
int motor1Strength;
int motor2Strength;
int motor3Strength;
int motor4Strength;

void setup()
{
  Serial.begin(9600);  
  pinMode(PPMInput, INPUT); //Pin 4 as input
 
  // make motor pins outputs
  for(int i = 0; i < 4; i++) {
    pinMode(allMotors[i], OUTPUT);   
  }
} 

void loop()
{
  //waits until synchronize arrives > 4 miliseconds
  // if pulse > 4 miliseconds, continues
  if(pulseIn(PPMInput , HIGH) > 4000) { 
    // Read the pulses of the remaining channels
    for(int i = 1; i <= numOfChannels; i++){
      channel[i-1]=pulseIn(PPMInput, HIGH);
    }
   
    // These inputs are based on the Fly Sky FSi6 Transmitter being set to Sticks Mode 2
    rollVal = map(channel[0], 590, 1590, -10, 11);
    pitchVal = map(channel[1], 590, 1600, -10, 11);
    thrustVal = map(channel[2], 590, 1580, 0, 20);
    yawVal = map(channel[3], 590, 1590, -10 , 11);
   
/*    // adjusting strength of motors, in this case for thrust
    strength = map(thrustVal, 0, 20, 0, 255);
    for (int i = 0; i < 4; i++) {
      analogWrite(allMotors[i], strength); 
    }
    */

    motor1Strength = thrustVal - pitchVal - rollVal + yawVal;
    motor1Strength = map(motor1Strength, -30, 50, 0, 255);
    analogWrite(motor1Pin, motor1Strength);
    
    motor2Strength = thrustVal - pitchVal + rollVal - yawVal;
    motor2Strength = map(motor2Strength, -30, 50, 0, 255);
    analogWrite(motor2Pin, motor2Strength);
    
    motor3Strength = thrustVal + pitchVal - rollVal - yawVal;
    motor3Strength = map(motor3Strength, -30, 50, 0, 255);
    analogWrite(motor3Pin, motor3Strength);
    
    motor4Strength = thrustVal + pitchVal + rollVal + yawVal;
    motor4Strength = map(motor4Strength, -30, 50, 0, 255);
    analogWrite(motor4Pin, motor4Strength);
 }  
}  

/*
 * Many thanks to jordi at http://forum.arduino.cc/index.php?topic=7167.0 for the help 
 *   in reading PPM signals from the receiver
 */

