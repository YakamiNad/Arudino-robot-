/*First implementation. 
Construct the bluetooth connection successfully. Bluetooth connection will be used for the whole project as requirement, we will transmit the data from master to slave or backwards frequently. The code is  provided. */

/* Second implementation.
Add the code for the first function. This function is used to detect if there is anyone break into house. The result that IR detector get will keep transmitting from slvae to master board. In master board, when the data received from slave board become positive result, trigger the buzzer to ring. */

/* Third implementation.
CHange the code for first function. The data received from slave board is no more IR detector's result, but temperature result. If temperature received on slave board is high, the character red will be sent to master board and make the RGB on this board red. If temperature is low then RGB is blue. If temperature is normal set the RGB to green.
*/

#include <SoftwareSerial.h>                     // Software Serial Port

#define RxD 7
#define TxD 6

#define DEBUG_ENABLED  1

// Make sure you modify this address to the one in your Slave Device!!
String slaveAddr = "0,6A,8E,16,C7,E7";

SoftwareSerial blueToothSerial(RxD,TxD);

void setup()
{
//set up bluetooth connection
  Serial.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBlueToothConnection();
  //wait 1s and flush the serial and btooth buffers
  delay(1000);
  Serial.flush();
  blueToothSerial.flush();
  
}

void loop()
{
  
  char recvChar;


  // Infinite loop
  while(1)
  {
    // If there is data pending to be read from the shield
    if(blueToothSerial.available())
    {
    //the character passed from slave board
      recvChar = blueToothSerial.read();
      // Print the data through the Serial channel
      Serial.print("Master Received: ");
      Serial.print(recvChar);
      Serial.print("\n");

// if character is 'r', which means temperature is high, set RGB to red to 
// warn the user
      if(recvChar == 'r'){
  analogWrite(11,255);
  analogWrite(10,0);
  analogWrite(9,0);
}
//if character is 'b', which means temperature is low, set RGB to blue to 
//warn the user
else if(recvChar == 'b'){
   analogWrite(11,0);
  analogWrite(10,0);
  analogWrite(9,255);
}
// if character is 'g', which means temperature is normal, set RGB to green
else if(recvChar == 'g'){
   analogWrite(11,0);
  analogWrite(10,255);
    analogWrite(9,0);
    }

    // If there is data pending to be read from the serial port
    if(Serial.available())
    {
      recvChar  = Serial.read();
      // Send the data through btooth
      blueToothSerial.print(recvChar);
      Serial.print("Master Transmitted: ");
      Serial.print(recvChar);
      Serial.print("\n");
    }
  }
}

// Function to start the connection. Make sure slaveAddr is set to the right
// value.
void setupBlueToothConnection()
{
  // Set BluetoothBee BaudRate to default baud rate 38400
  blueToothSerial.begin(38400);
  // set the bluetooth work in master mode
  blueToothSerial.print("\r\n+STWMOD=1\r\n");
  // set the bluetooth name (irrelevant)
  blueToothSerial.print("\r\n+STNA=Master20\r\n");
  // Auto-connection is forbidden here
  blueToothSerial.print("\r\n+STAUTO=0\r\n");
  // This delay is required.
  delay(2000);
  blueToothSerial.flush();
  // This delay is required.
  delay(2000);

  //form the full connection command
  Serial.print("Connecting to slave: ");
  Serial.println(slaveAddr);
  //send connection command
  blueToothSerial.print("\r\n+CONN=" + slaveAddr + "\r\n");
  delay(5000);
}

