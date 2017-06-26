 // includes
#include <SdFat.h>

// SiDrive commands
#define cmdSiS1 0xA0 //SiDrive status Drive 1
#define cmdSiS2 0xA2 //SiDrive status Drive 2
#define cmdSiR1 0xA4 //SiDrive Read Drive 1
#define cmdSiR2 0xA6 //SiDrive Read Drive 2
#define cmdSiW1 0xA8 //SiDrive Write Drive 1
#define cmdSiW2 0xAA //SiDrive Write Drive 2
#define cmdsync 0x80 // Sync packet ack with 0x81

// system constants
#define GLED  16 // Green LED
#define RLED  15 // Red LED

// MEGA serial port 2
#define MEGA
#ifdef MEGA
#define SiPort Serial1
#else
#define SiPort Serial
#endif

// globals
SdFat sd;
SdFile vol1, vol2;
char data[4];    // generic array for holding various things in vsdrive
byte block[513]; // holder for a block of disk data

void err(char *errstr)
{
#ifdef MEGA
  Serial.print(errstr);
  Serial.println(" error.");
#else
  digitalWrite(GLED, LOW);
  digitalWrite(RLED, HIGH);
#endif
  sd.errorHalt();
  while (1);
}

//
// Initialization
//
void setup()
{
#ifndef MEGA
  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  digitalWrite(GLED, HIGH);
#endif
  SiPort.begin(115200); 
  SiPort.setTimeout(15);
#ifdef MEGA
  Serial.begin(9600);
  if (!sd.begin(4, SPI_FULL_SPEED)) err("Can't enable SDcard"); //pin 4 when using ethernet shield
#else
  if (!sd.begin(SS, SPI_FULL_SPEED)) err("Can't enable SDcard"); // SS on VDriveSSC
#endif
  if (!vol1.open("Virtual.po",  O_RDWR | O_SYNC)) err("Can't open Virtial.po");
  if (!vol2.open("Virtual2.po", O_RDWR | O_SYNC)) err("Can't open Virtual2.po");
}

// Global Functions
void access(byte acc) // LED blinking
{
#ifndef MEGA
  if(acc) 
  {
    digitalWrite(GLED, LOW);
    digitalWrite(RLED, HIGH);
  }
  else
  {
    digitalWrite(RLED, LOW);
    digitalWrite(GLED, HIGH);
  }
#endif
}

int ReadSerial()
{
  if (SiPort.available())
    return byte(SiPort.read());
  return -1;
}

// SiDrive commands
void SiStat(SdFile *vol)
{
  unsigned int blockSize;
  
  SiPort.readBytes(data,2);  // Read 2 bytes, dummy data
  blockSize = vol->fileSize() >> 9; // div by 512
  data[0] = blockSize;
  data[1] = blockSize >> 8;
  SiPort.write(data, 2); // send volume file size
  SiPort.write(blockSize ? 0x00 : 0xA8);
}

void SiRead(SdFile *vol)
{
  SiPort.readBytes(data,2);  // Read 2 byte block num
  access(1);
  // set file pointer
  vol->seekSet((word(data[1], data[0]) * 512UL));
  vol->read(block,512); // read block data
  SiPort.write(block,512); // send block data
  access(0);
  SiPort.write(0x00); // send checksum
}

void SiWrite(SdFile *vol)
{
  SiPort.readBytes(data,2);  // Read 2 byte block num
  SiPort.readBytes((char*)block, 512); // read block data and checksum
  access(1);
  // write data to SD card
  vol->seekSet((word(data[1], data[0]) * 512UL)); 
  vol->write(block,512);
  access(0);
  SiPort.write(0x00);  // return 0x00
}

//
// Main command handler
//
void loop()
{
  int cmd;
  
  if ((cmd = ReadSerial()) >= 0)
  {
#ifdef MEGA
    Serial.println(cmd, HEX);
#endif
    if (cmd) SiPort.write(cmd+1);       // Acknowledge command
    switch(cmd)
    {
      case cmdSiR1:
        SiRead(&vol1);
        break;
      case cmdSiR2:
        SiRead(&vol2);
        break;
      case cmdSiW1:
        SiWrite(&vol1);
        break;
      case cmdSiW2:
        SiWrite(&vol2);
        break;
      case cmdSiS1:
        SiStat(&vol1);
        break;
      case cmdSiS2:
        SiStat(&vol2);
        break;
      case cmdsync:
        vol1.sync();
        vol2.sync();
        break;
    }
    SiPort.flush();
  }
}

