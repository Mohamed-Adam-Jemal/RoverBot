//Code written by: Mohamed Adam Jemal

//Disclaimer 

/*
  This Arduino code for the car robot is open source and free to use, modify, and distribute.
  
  You are welcome to:
  - Use this code for personal or commercial projects.
  - Modify and adapt this code to suit your needs.
  - Share this code with others.
  
  If you distribute or share this code, please retain this disclaimer.  
  
  Thank you for contributing to the open-source community!
*/



//////////////////////////////////////////////////// Inclusion of libraries////////////////////////////////////////////////


#include <AFMotor.h>   // Include the AFMotor library for motor control
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>   // Include the Dabble library for Bluetooth Communication
#include <Servo.h>   // Include the Servo library for servo motor control //
#include <NewPing.h> // Include the NewPing library for the ultrasonic sensor //


//////////////////////////////////////////////////// Creating  Objects ////////////////////////////////////////////////////


NewPing uls_sensor1(A0, A1, 400);   /* Create an object of the NewPing class for ultrasonic sensor with the AnalogPin 0 as Trig pin and the AnalogPin1 as 
an EchoPin and a max distance of 400cm */

Servo servo1;  // Create a Servo object for the servo motor

AF_DCMotor motor1(1);	// Create AF_DCMotor object for motor1
AF_DCMotor motor2(2);	// Create AF_DCMotor object for motor2
AF_DCMotor motor3(3);	// Create AF_DCMotor object for motor3
AF_DCMotor motor4(4);	// Create AF_DCMotor object for motor4


/////////////////////////////////////////////////// Movement  functions ///////////////////////////////////////////////////


// Moving forward function 

void forward() {
  motor1.run(FORWARD);      
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);    
}

// Moving backward function 

void backward() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

// Turning right function 

void right() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

// Turning left function 

void left() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

// Function to stop all RoverBot motors

void stop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}


/////////////////////////////////////////////////// Distance measurement functions ///////////////////////////////////////


// Function to measure the forward distance using the ultrasonic sensor

unsigned int mes_dist_forward() {
  unsigned int distance = uls_sensor1.ping_cm();
  return distance;  
}

// Function to measure the right distance using the servo and ultrasonic sensor 

int mes_dist_right() {
  servo1.write(10);
  delay(1000);
  unsigned int dist_right = uls_sensor1.ping_cm();
  delay(300);
  servo1.write(90);
  return dist_right; 
}

// Function to measure the left distance using the servo and ultrasonic sensor 

int mes_dist_left() {
  servo1.write(170);
  delay(1000);
  unsigned int dist_left = uls_sensor1.ping_cm();
  delay(300);
  servo1.write(90);
  return dist_left; 
}

// Function to choose between turning to the right or to the left based on the longest distance captured by the ultrasonic Sensor
void search() {
  if (mes_dist_right() >= mes_dist_left()) {
    right();
    delay(350);
    forward();
  } else {
    left();
    delay(350);
    forward();
  }
}

/////////////////////////////////////////////////// Setup function /////////////////////////////////////////////

void setup() {
  Dabble.begin(9600, 0, 1);  // Initialize Dabble for Bluetooth communication
  motor1.setSpeed(255);  // Set motors speeds
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);
  servo1.attach(10);  // Attach the servo to pin 10
  servo1.write(90);   // Set the initial position of the servo (90 degree)
}

///////////////////////////////////////////////////  Loop function /////////////////////////////////////////////

bool obs_mode = true; // Initialize RoverBot to the Autonomous Mode (Obstacles avoiding Mode)

void loop() {
  Dabble.processInput();  // Process Bluetooth inputs from Dabble app
  if (GamePad.isCrossPressed()) {
    obs_mode = false; // deactivate the Autonomous Mode
  } else if (GamePad.isTrianglePressed() || (obs_mode)) {
    obs_mode = true; // activate the Autonomous Mode
    if (mes_dist_forward() >= 33) {
      forward();
    } else {
      stop();
      delay(500);
      search();
      forward();
    }
} else {
    if ((GamePad.isUpPressed()) && (mes_dist_forward() >= 33)) {
      forward();
    } else if (GamePad.isDownPressed()) {
      backward();
    } else if (GamePad.isRightPressed()) {
      right();
    } else if (GamePad.isLeftPressed()) {
      left();
    } else {
      stop();
    }
  }
}