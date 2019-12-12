//First we Include Libraries for the wireless module library
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object which forms the wireless module
RF24 radio(9, 8);  // CE, CSN

//We specify the address through which both the wireless modules communicate, with a 5 digit password.
const byte address[6] = "00005";


#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

//  Variables
const int PulseWire = 0;       // PulseSensor firt cable connected to analog pin 0.
int LED;                       // We declare a varibale for the RGB led
int Threshold = 512;           // Determine which Signal to "count as a beat" and which to ignore, all the numbers below will this constant will be ignored.

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"


void setup()
{
  Serial.begin(9600);          // Begin the Serial Monitor at 9600


  pulseSensor.analogInput(PulseWire);//We configurate the pulseSensor module aranging variables
  pulseSensor.setThreshold(Threshold);

  // Begin the PulseSensor module and confirm with a sentence to have and output and make sure it is working.
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //Print this sentence as confirmation
  }
  radio.begin(); //Start the comunication between modules

  // We set the address defined before
  radio.openWritingPipe(address);

  //We set this module as the transmitter of information
  radio.stopListening();
}
void loop()
{
  int myBPM = pulseSensor.getBeatsPerMinute(); // We create the variable myBPM which counts the beats per minute of the user

  if (pulseSensor.sawStartOfBeat()) {            // Check if a heart beat is happening repeatedly
    Serial.println("♥ beat ♥"); // If a heart beat does happen, print the sentence shown before and the value of the BPM
    Serial.print("BPM: ");
    Serial.println(myBPM);
  }
  pulseSensor.blinkOnPulse(LED); // We declare that the RGB led should blink at the rhythm of the heart of the user.
  if (myBPM < 40) { // If the Beats per  minute are too low do the following:
    digitalWrite(5, LOW); //Green led of the RGB led turns off
    digitalWrite(3, LOW);//Red led of the RGB led turns off
    LED = 6; //BLue led of the RGB led turns on
  } else if (myBPM > 120) {// If the Beats per  minute are too high do the following:
    digitalWrite(5, LOW);//Green led of the RGB led turns off
    digitalWrite(6, LOW);//BLue led of the RGB led turns off
    LED = 3;//Red led of the RGB led turns on
  } else {// If the Beats per  minute are normal do the following:
    digitalWrite(6, LOW);//BLue led of the RGB led turns off
    digitalWrite(3, LOW);//Red led of the RGB led turns off
    LED = 5;//Green led of the RGB led turns on
  }
  delay(10); // Wait for 10
  {
    radio.write(&myBPM, sizeof(myBPM)); //Write the value of BPM on the wireless module, to comunicate with the reciever
    delay(100);// Wait for 100
  }
}
