/*
  Parallax RFID Reader
 language: Processing
 */

// import the serial library 
import processing.serial.*;

Serial rfidPort;      // the serial port you're using
String tagID = "";  // the string for the tag ID

void setup() {
  size(600, 200);
  // list all the serial ports 
  println(Serial.list());

  // based on the list of serial ports printed from the
  // previous command, change the 0 to your port's number 
  String portnum = Serial.list()[0];

  // initialize the serial port 
  rfidPort = new Serial(this, portnum, 2400);

  // incoming string from reader will have 12 bytes:
  rfidPort.buffer(12);

  // create a font with the third font available to the system:
  PFont myFont = createFont(PFont.list()[2], 24);
  textFont(myFont);
}

void draw() {
  // clear the screen:
  background(0);

  // print the string to the screen 
  text(tagID, width/4, height/2 - 24);
}

/*
 this method reads bytes from the serial port
 and puts them into the tag string.
 It trims off the \r and \n
 */
void serialEvent(Serial rfidPort) { 
  tagID = trim(rfidPort.readString());
}

