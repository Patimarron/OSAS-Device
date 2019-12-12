//First we Include Libraries for the wireless module library
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object which forms the wireless module
RF24 radio(9, 8);  // CE, CSN

//We specify the address through which both the wireless modules communicate, with a 5 digit password.
const byte address[6] = "00005";
//We declare a variable for the motor of vibration which is connected to pin number 10
int motorvibracion = 10;

void setup()
{
  while (!Serial);
  Serial.begin(9600);// Begin the Serial Monitor at 9600

  radio.begin();//Start the comunication between modules

  // We set the address defined before
  radio.openReadingPipe(0, address);

  //We set this module as the reciber of information
  radio.startListening();

  //We declare the vibration monitor as an output
  pinMode(motorvibracion, OUTPUT);
  //The vibration motor starts turned of
  digitalWrite(motorvibracion, LOW);
}

void loop()
{
  analogWrite(motorvibracion, 0); // Vibration motor its still
  int myBPM; // We declare the variable BPM
  //Read the data if available in buffer
  if (radio.available())
  {
    //We read the iformation reciebed from the transmitter of the BPM of the user
    radio.read(&myBPM, sizeof(myBPM));
    //Print the value of the variable
    Serial.println(myBPM);
  }//If BPM is equal to 0 do not do anything because it is useless information
  if (myBPM == 0) {

  } else if (myBPM < 40) {// If the Beats per  minute are too low do the following:
    digitalWrite(5, LOW);//Green led of the RGB led turns off
    digitalWrite(3, LOW);//Red led of the RGB led turns off
    digitalWrite (6, HIGH);//BLue led of the RGB led turns on
    analogWrite(motorvibracion, 250);//The vibration motor turns on to a power of 250(which is the maximun)
  } else if (myBPM > 120) {// If the Beats per  minute are too high do the following:
    digitalWrite(5, LOW);//Green led of the RGB led turns off
    digitalWrite(6, LOW);//BLue led of the RGB led turns off
    digitalWrite (3, HIGH);//Red led of the RGB led turns on
    analogWrite(motorvibracion, 200);//The vibration motor turns on to a power of 200
  } else if (myBPM > 40 && myBPM < 120) {//If the Beats per  minute are normal do the following:
    digitalWrite(3, LOW);//Red led of the RGB led turns off
    digitalWrite(6, LOW);//BLue led of the RGB led turns off
    digitalWrite (5, HIGH);//Green led of the RGB led turns on
    analogWrite(motorvibracion, 0);//The vibration motor turns off 
  }
}
