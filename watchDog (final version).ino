/*First implementation. 
Construct the bluetooth connection successfully. Bluetooth connection will be used for the whole project as requirement, we will transmit the data from master to slave or backwards frequently. The code is  provided. 
And add the code for the car to run forward, backward, turn left and turn right. The command that controls car to move is recieved from master board through bluetooth connection. This function enable the car to move while we send the command from master. */

/*bluetooth connection: First declare the pin we will use for bluetooth connection which is 6 and 7. Second, in the setup block, set the serial begin in 9600, then set the input output pin using 7 and 6, do the setup bluetooth connection. Wait for the flush. 
In the loop, declare new variable recvChar, and create an infinite loop. If the bluetooth receive any word from master board, it will satisfy the if(blueToothSerial.available()) condition and read the word and turn to uppercase then send back. Though in this design we don't need to turn the command to uppercase, we still save that as a test. If the uppercase word send back, that means the command access in this if loop successfully. It's helpful for detecting the connection in later implementation.
This function makes car can transmit and receive data with master through bluetooth. It will be used frequently in later implementation. */

/* Moving car: First declare left and right servos for servo motor. 
In the loop, if there is data received from master board through bluetooth, it will turn to uppercase and send back. After send back the letter, read the recvChar variable. If it is "W", that means master send a "w". Then make the car moving forward. If it is "S", make the car moving backward. If it is "A", turn the car left. If it is "D", turn the car right. Because when play RPG game, we always use WASD to move player, that's why we think we can also use WASD to move the car.
*/

/* Second implementation. 
Add the code for the first function. This function is used to detect if there is anyone break into house. That is, use IR detector to detect and send the result back to master board. IR detecter receives positive result if someone is entering the house. Slave board will keep sending the value that IR detector get to the master board. If master board receives a positive result, which means someone breaks in, it will trigger the buzzer to ring. */

/* Guarding mode (old version): In the loop, first define a boolean variable called "guard" and initiallised it to false. This is used to switch on/off the guarding mode. "guard" will be controled by the data received from master board through bluetooth connection. If master board send "g", recvChar in loop will become "G". In the loop, if recvChar detected as "G", "guard" will be true. Then the car will start the guarding mode and IR detector will begin to send back the result it got. If we want to switch off the guarding mode, just type "b" in master board and send to slave board. Then "guard" will be false, the IR detector stop working.
*/

/* Third implementation.
In the third implementation, we make some changes in the design. In previous period, the data recieved by the IR detector are transmitted to master board to trigger the buzzer. In this phase, we remove the buzzer from master board and add to the car. Thus, the data received by the IR detector will control the buzzer directly. And we also add two functions. 
One is mopping the floor by using DC motor and moving function. We stick a mop on the DC motor. And set a boolean that can switch on/off of the motor. If master board set the boolean true, the motor will start rotating. We can move the car around the house while motor keep rotating and mop the floor.
Another one is temperature notification. By using temperature sensor on the car, the data will be sent to master board and set the RGB on the master board to notify the user.
*/

/* Guarding mode (current version): The basic idea is the same as before. Though the data detected by the IR detector won't send to master board through bluetooth. Instead, the data will directly used on slave board to control the buzzer. When IR detector got positive result, the buzzer will ring in a warning tone. */

/* Mopping:
First set the pin 5 as motor's output pin. In the loop, declare a boolean variable called "motor" and initialise it to false. This "motor" variable is similar to "guard" above, and can be controled by the input from master board. If master board type "m", the motor will set to be true and the motor function outside the bluetooth block will be executed. If master board type "n", the motor will set to be false and stop executing the motor function and DC motor stop rotating.
*/

/* Temperature notification:
First set the analog pin A0 as analog pin for temperature sensor. And also declare a integer called "val" and initialised it to 0.
Then in the loop, integer "val" will keep being modified by the input from analog pin A0, which is the input value from temperature sensor. Then create a new float called "tem" that use formula to convert the data to Celcius degree. And every time we got the "tem", check its value and deside if it is a high temperature or low temperature or normal. Create a character and send it back to master board to control the RGB light on master board.
*/

#include <SoftwareSerial.h>   //Software Serial Port
#include <Servo.h>                      // Include servo library

Servo servoLeft;                        // Declare left and right servos

Servo servoRight;

#define RxD 7
#define TxD 6

#define DEBUG_ENABLED  1

SoftwareSerial blueToothSerial(RxD,TxD);

int analogPin = A0;     // potentiometer wiper (middle terminal) connected to //analog pin 

// declare new integer for input of temperature sensor
int val = 0;      

// use integer to remember the pin used by motor is pin 5
int motorPin = 5;

void setup() 
{ 
//set up the bluetooth connection
  Serial.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBlueToothConnection();

  //wait 1s and flush the serial and btooth buffers
  delay(1000);
  Serial.flush();
  blueToothSerial.flush();

  //used to test if setup process successfully
tone(8, 3000, 1000);                       // Play tone for 1 second
  delay(1000);                               // Delay to finish tone
  
  Serial.begin(9600);                        // Set data rate to 9600 bps
  
// set the motor pin to output
  pinMode(motorPin,OUTPUT);
  
} 

void loop() 
{ 
//character that reads in value from master board through bluetooth
  char recvChar;

  //boolean that control the guarding mode
  boolean guard=false;

  //boolean that control the motor mode
  boolean motor = false;

  // Infinite loop
  while(1){
     val = analogRead(analogPin);    // read the input pin

     //convert the value to temperature in Celcius degree
   float tem = ((val*5.0/1024)-0.5)*100;

   //declare a new character to represent the degree
   char temperature;

   //if temperature is lower than 15, which means temperature is low, 
   //send back the character 'b' to trigger RGB on master board to blue
   if(tem<15){
   temperature = 'b';
   }
// if temperature is greater than 20, which means temperature is high, 
//send back the character 'r' to trigger RGB on master board to red
   else if (tem>20){
   temperature = 'r';
   }
 // if temperature is between 15 and 20, which means temperature is normal,
 //send back the character 'g' to trigger RGB on master board to green
   else{
   temperature = 'g';
   }

  // transmit the character through bluetooth
   blueToothSerial.print(temperature);

// If there is data ready in the btooth

    if(blueToothSerial.available())
    {

      // Read, turn to uppercase if appropriate and send back
      recvChar = blueToothSerial.read();
      Serial.print("Slave Received: ");
      Serial.print(recvChar);
      Serial.print("\n");
      if (recvChar >= 'a' && recvChar <= 'z')
      {
        // Make the character upper case
        recvChar -= 32;
      }
      // Send back
      blueToothSerial.print(recvChar);
      Serial.print("Slave Transmitted: ");
      Serial.print(recvChar);
      Serial.print("\n");

      // if received character is 'g', boolean set to be true and guard mode 
      //begins.
      if(recvChar =='G'){
        guard = true;
      }

      //if received character is 'b', boolean set to be false and guard mode
      //stops
      if(recvChar == 'B'){
        guard = false;
      }

      //if master send "w", that's the command for car to move forward
      if(recvChar == 'W')
      {
         servoLeft.attach(13);               // Attach left signal to pin 13
    servoRight.attach(12);              // Attach right signal to pin 12

    // Full speed forward
    servoLeft.writeMicroseconds(1700);  // Left wheel counterclockwise
    servoRight.writeMicroseconds(1300); // Right wheel clockwise
    delay(3000);                        // ...for 3 seconds
    servoLeft.detach();                 // Stop sending servo signals
    servoRight.detach();
      }


      //if master send "a", that's the command for car to turn left
      if(recvChar == 'A')
      {
         servoLeft.attach(13);               // Attach left signal to pin 13
    servoRight.attach(12);              // Attach right signal to pin 12

    // turn left
    servoLeft.writeMicroseconds(1700);  // Left wheel counterclockwise
    servoRight.writeMicroseconds(1500); // Right wheel not moving
    delay(3000);                        // ...for 3 seconds
    servoLeft.detach();                 // Stop sending servo signals
    servoRight.detach();
      }

      //if master send "d", that's the command for car to turn right
            if(recvChar == 'D')
      {
        servoLeft.attach(13);               // Attach left signal to pin 13
    servoRight.attach(12);              // Attach right signal to pin 12

    // turn right
    servoLeft.writeMicroseconds(1500);  // Left wheel not moving
    servoRight.writeMicroseconds(1300); // Right wheel clockwise
    delay(3000);                        // ...for 3 seconds
    servoLeft.detach();                 // Stop sending servo signals
    servoRight.detach();
      }

       //is master send "s", that 's the command for moving backward
      if(recvChar == 'S'){
         servoLeft.attach(13);               // Attach left signal to pin 13
    servoRight.attach(12);              // Attach right signal to pin 12

    // Full speed backward
    servoLeft.writeMicroseconds(1300);  // Left wheel clockwise
    servoRight.writeMicroseconds(1700); // Right wheel counterclockwise
    delay(3000);                        // ...for 3 seconds
    servoLeft.detach();                 // Stop sending servo signals
    servoRight.detach();
      }

      //if received character is 'm', set the boolean to true and motor mode
      //starts
      if(recvChar == 'M'){
      motor = true;
    }
    //if received character is 'n', set the boolean to false and motor mode 
    //stops
    if(recvChar == 'N'){
      motor = false;
    }
  }

  //outside the bluetooth condition, if guard boolean is true, execute this
  //block
     if(guard)
      {
      //integer received by IR detector
         int irLeft = irDetect(9, 10, 38000); 
         delay(700);// Check for object
         Serial.println(irLeft);

         //check if value received is 0
        if(irLeft == 0){

        //trigger the buzzer to ring
           tone(8,1800,250);
        delay(300);
        tone(8,1900,250);
        delay(300);
        tone(8,2000,250);
        delay(300);
        tone(8,2100,250);
        delay(300);
        tone(8,2200,250);
        delay(300);
        tone(8,2300,250);
        delay(300);
        tone(8,2400,250);
        delay(300);
        tone(8,2500,250);
        delay(300);
        tone(8,2600,250);
        delay(300);
        tone(8,2700,250);
        delay(300);
        tone(8,2800,250);
        delay(300);
        tone(8,2900,250);
        delay(300);
        tone(8,3000,250);
        delay(300);
        tone(8,3100,250);
        delay(300);
        }
      }

      // if motor boolean is true, execute this block
      if(motor){
      // start the DC motor
         digitalWrite(motorPin,HIGH);
      }
}
} 

void setupBlueToothConnection()
{
  //Set BluetoothBee BaudRate to default baud rate 38400
  blueToothSerial.begin(38400);
  //set the bluetooth work in slave mode
  blueToothSerial.print("\r\n+STWMOD=0\r\n");
  //set the bluetooth name (irrelevant)
  blueToothSerial.print("\r\n+STNA=Slave20\r\n"); 
  // Permit Paired device to connect me
  blueToothSerial.print("\r\n+STOAUT=1\r\n");
  // Auto-connection is not allowed
  blueToothSerial.print("\r\n+STAUTO=0\r\n"); 
  delay(2000); // This delay is required.
  //make the slave bluetooth inquirable 
  blueToothSerial.print("\r\n+INQ=1\r\n");
  delay(5000); // This delay is required.
}

int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency, 8);              // IRLED 38 kHz for at least 1 ms
  delay(1);                                  // Wait 1 ms
  int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
  delay(1);                                  // Down time before recheck
  return ir;                                 // Return 1 no detect, 0 detect
}  

