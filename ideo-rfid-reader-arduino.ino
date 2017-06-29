#include <SoftwareSerial.h>

SoftwareSerial rSerial(2, 3); // RX, TX

const int tagLen = 16;
const int idLen = 13;
const int kTags = 4;

// Empty array to hold a freshly scanned tag
char rfidTag[idLen];

void setup() {
  // Starts the hardware and software serial ports
  Serial.begin(9600);
  rSerial.begin(9600);
}

void loop() {
  
  // Counter for the rfidTag array
  int i = 0;
  
  // Variable to hold each byte read from the serial buffer
  int readByte;
  
  // Flag so we know when a tag is over
  boolean tag = false;

  // This makes sure the whole tag is in the serial buffer before
  // reading, the Arduino can read faster than the ID module can deliver!
  if (rSerial.available() == tagLen) {
    tag = true;
  }

  if (tag == true) {
    while (rSerial.available()) {
      // Take each byte out of the serial buffer, one at a time
      readByte = rSerial.read();

      /* This will skip the first byte (2, STX, start of text) and the last three,
      ASCII 13, CR/carriage return, ASCII 10, LF/linefeed, and ASCII 3, ETX/end of 
      text, leaving only the unique part of the tag string. It puts the byte into
      the first space in the array, then steps ahead one spot */
      if (readByte != 2 && readByte!= 13 && readByte != 10 && readByte != 3) {
        rfidTag[i] = readByte;
        i++;
      }

      // If we see ASCII 3, ETX, the tag is over
      if (readByte == 3) {
        tag = false;
      }
    }
  }

  // don't do anything if the rfidTag array is full of zeroes
  if (strlen(rfidTag) == 0) {
    return;
    
  } else {    
    Serial.println(rfidTag);
  }

  zeroTagBuffer();
}

// Once rfidTag has been processed, fill it with
// zeros to get ready for the next tag read
void zeroTagBuffer() {
  for (int c=0; c < idLen; c++) {
    rfidTag[c] = 0;
  }
}
