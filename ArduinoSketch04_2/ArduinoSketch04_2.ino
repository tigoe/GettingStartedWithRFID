/*
 RFID–to-PowerSwitch Tail control
 language: Wiring/Arduino
 
 */
#include <SoftwareSerial.h> // Bring in the software serial library 

const int tagLength = 10;    // each tag ID contains 10 bytes
const int startByte = 0x0A;  // Indicates start of a tag
const int endByte   = 0x0D;  // Indicates end of a tag

const int rxpin = 6; // Pin for receiving data from the RFID reader
const int txpin = 7; // Transmit pin; not used

String currentTag;   // String to hold the tag you're reading 

String tag[] = { "04162F7CAC", "0415EA09BE"}; // List of tags 
int numTags = 2;                              // Number in that list

// PowerSwitchTail unit pins and unit states: 
int numUnits    = 2;          // Two PowerSwitch Tails
int unit[]      = {2, 3};     // Pins 2 and 3
int unitState[] = {
  LOW, LOW}; // Both start in the off position

long lastRead;       // the time when we last read a tag
long timeOut = 1000; // required time between reads

SoftwareSerial rfidPort(rxpin, txpin); // create a Software Serial port

void setup() {
  lastRead = millis(); // Initalize to the sketch's start time 

  // begin serial communication with the computer
  Serial.begin(9600);

  // begin serial communication with the RFID module
  rfidPort.begin(2400);

  // Initialize all the PowerSwitchTail controller pins. 
  for (int thisUnit = 0; thisUnit < numUnits; thisUnit++) {
    pinMode(unit[thisUnit], OUTPUT); // Enable this pin for output
    digitalWrite(unit[thisUnit], unitState[thisUnit]);
  }
}

void loop() {
  // read in and parse serial data: 
  if (rfidPort.available()) {
    readByte();
  }
}

void readByte() {
  char thisChar = rfidPort.read(); // Read a character from the port 

  // depending on the byte's value,
  // take different actions:
  switch(thisChar) {
    // if the byte == startByte, you're at the beginning 
    // of a new tag:
  case startByte:
    currentTag = "";
    break;
    //if the byte == endByte, you're at the end of a tag: 
  case endByte:
    checkTags();
    break;
    // other bytes, if the current tag is less than  
    // 10 bytes, you're still reading it:
  default:
    if (currentTag.length() < 10) {
      currentTag += thisChar;
    }
  }
}

void checkTags() {
  // iterate over the list of tags: 
  for (int thisTag = 0; thisTag < numTags; thisTag++) {

    // if the current tag matches the tag you're on: 
    if (currentTag.equals(tag[thisTag])) {

      // Only flip a switch if the tag has been away for a while
      if (lastRead + timeOut < millis()) {

        // unit number starts at 1, but list position starts at 0:
        Serial.print("unit " + String(thisTag +1));

        // change the status of the corresponding unit: 
        if (unitState[thisTag] == HIGH) {
          unitState[thisTag] = LOW;
          Serial.println(" turning OFF");
        }
        else {
          unitState[thisTag] = HIGH;
          Serial.println(" turning ON");
        }

        // Set the switch to the new state. 
        digitalWrite(unit[thisTag], unitState[thisTag]);
      }
      lastRead = millis(); // mark the last time you got a good tag 
    }
  }
}

