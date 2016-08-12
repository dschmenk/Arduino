/*
 * Operate on FAT files on SD card from Apple II
 */
#include <SPI.h>
#include <FreeStack.h>
#include <MinimumSerial.h>
#include <SdFat.h>
#include <SdFatConfig.h>
#include <SdFatUtil.h>
#include <SystemInclude.h>
const int sdSSpin = 4;
/*
 * Bit bang SPI slave mode for Apple II
 */
const int SSpin   = 3; // PORTD.3 for Uno
const int SCKpin  = 6; // PORTD.6 for Uno
const int MISOpin = 7; // PORTD.7 for Uno
const int MOSIpin = 8; // PORTB.0 for Uno
/*
 * SPI status
 */
const int SLAVE_BUSY  = 0xFF;
const int SLAVE_READY = '@';
const int SLAVE_ERROR = '!';
const int SLAVE_ECHO  = -1;
/*
 * Erros codes
 */
const int SLAVE_ERR_OK      = 0;
const int SLAVE_ERR_TIMEOUT = -1;
const int SLAVE_ERR_BADVAL  = -2;
const int SLAVE_ERR_WAITING = -3;
/*
 * SPI commands
 */
const int SLAVE_CMD_NOP      = 0;
const int SLAVE_CMD_ECHO     = 1;
const int SLAVE_CMD_HW       = 2;

const int SLAVE_CMD_PINMODE  = 3;
const int SLAVE_CMD_DIGREAD  = 4;
const int SLAVE_CMD_DIGWRITE = 5;
const int SLAVE_CMD_ANAREAD  = 6;
const int SLAVE_CMD_PWMWRITE = 7;

const int SLAVE_CMD_SERMODE  = 8;
const int SLAVE_CMD_SERAVAIL = 9;
const int SLAVE_CMD_SERREAD  = 10;
const int SLAVE_CMD_SERWRITE = 11;

const int SLAVE_CMD_BUFREAD  = 12;
const int SLAVE_CMD_BUFWRITE = 13;

const int SLAVE_CMD_SDINIT   = 14;
const int SLAVE_CMD_SDCHDIR  = 15;
const int SLAVE_CMD_SDMKDIR  = 16;
const int SLAVE_CMD_SDRMDIR  = 17;
const int SLAVE_CMD_SDRM     = 18;
const int SLAVE_CMD_SDREN    = 19;
const int SLAVE_CMD_SDOPEN   = 20;
const int SLAVE_CMD_SDOPENNEXT=21;
const int SLAVE_CMD_SDCLOSE  = 22;
const int SLAVE_CMD_SDREAD   = 23;
const int SLAVE_CMD_SDWRITE  = 24;
/*
 * How log to wait before checking again
 */
const int SLAVE_CMD_TIMEOUT  = 1000;
const int SLAVE_DATA_TIMEOUT = 10000;
const int SLAVE_TIMEOUT_USEC = 10;
/*
 * Arduino models
 */
const int HW_MODEL_UNO    = 0x01;
const int HW_MODEL_MEGA   = 0x02;
/*
 * Data transfer buffer
 */
 const int MAX_BUF_SIZE = 512;
  byte xferBuf[MAX_BUF_SIZE];
/*
 * FAT file entries. One for directory, one for file
 */
SdFat sdFat;
SdFile sdFile;
bool sdInit = false;
/* 
 *  Bit banged SPI data transfer variables
 */
volatile byte spiInput;
volatile byte spiAvail = false;
          byte spiOutput = SLAVE_BUSY;
/*
 * Initialize it all
 */
void setup(void)
{
  pinMode(SCKpin,  INPUT);
  pinMode(SSpin,   INPUT);
  pinMode(MOSIpin, INPUT);
  pinMode(MISOpin, OUTPUT);
  digitalWrite(MISOpin, LOW) ;
  attachInterrupt(digitalPinToInterrupt(SSpin), spiXfer, FALLING);
  Serial.begin(9600);
  spiReady();
}
//
// Shift in/out serial data
//
const int    SLAVE_CLK_TIMEOUT = 0xFF;
#define SCLK_FALL for(timeout=SLAVE_CLK_TIMEOUT;PIND&0x40&&timeout--;)
#define SCLK_RISE for(timeout=SLAVE_CLK_TIMEOUT;!(PIND&0x40)&&timeout--;)

void spiXfer(void)
{
  byte outPort, spiSR;
  byte timeout;
  
  outPort = PORTD & 0x7F;
  spiSR   = spiOutput;
  SCLK_FALL;
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
  SCLK_RISE;
  SCLK_FALL;
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
  SCLK_RISE;
  SCLK_FALL;
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
  SCLK_RISE;
  SCLK_FALL;
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
  SCLK_RISE;
  SCLK_FALL;
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
  SCLK_RISE;
  SCLK_FALL;
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
  SCLK_RISE;
  SCLK_FALL;
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
  SCLK_RISE;
  SCLK_FALL;
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
   if (!spiAvail)
   {
    spiAvail  = true;
    spiInput  = spiSR;
    spiOutput = SLAVE_BUSY; 
  }
}
/*
 * SPI slave to Apple II
 */
int spiReadByte(int timeout, int nextOut)
{
  byte readByte;
  
  do {
    if (spiAvail)
    {
      spiOutput = nextOut == SLAVE_ECHO ? spiInput : nextOut;
      readByte  = spiInput;
      spiAvail  = false;
      return readByte;
    }
    delayMicroseconds(SLAVE_TIMEOUT_USEC); 
  } while (timeout--);
  return SLAVE_ERR_TIMEOUT;
}
int spiWriteByte(byte val, int timeout)
{
  byte readByte;
  
  noInterrupts();
  spiOutput = val;
  spiAvail  = false;
  interrupts();
  while (timeout--)
  {
    if (spiAvail)
    {
      readByte = spiInput;
      spiAvail = false;
      return readByte;
    }
    delayMicroseconds(SLAVE_TIMEOUT_USEC); 
  }
  return SLAVE_ERR_TIMEOUT;
}
void spiBusy(void)
{
  spiOutput = SLAVE_BUSY;
  PORTD |= 0x80;
}
void spiReady(void)
{
  noInterrupts();
  spiOutput = SLAVE_READY;
  spiAvail  = false;
  interrupts();
}
void loop(void)
{
  int cmd, pin, data, count;
  File entry;
  
  cmd = spiReadByte(0, SLAVE_ECHO);
  if (cmd >= 0)
  {
    switch (cmd)
    {
      case SLAVE_CMD_NOP:
        spiReady();
        break;
      case SLAVE_CMD_ECHO:
        data = spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_READY);
        if (data >= 0)
          spiWriteByte(data, SLAVE_CMD_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_HW:
        spiWriteByte(HW_MODEL_UNO , SLAVE_CMD_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_PINMODE:
        pin  = spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_READY);
        data = spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_READY);
        if (pin >=0 && data >= 0)
          pinMode(pin, data);
        break;
      case SLAVE_CMD_DIGREAD:
        pin  = spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_BUSY);
        spiWriteByte(digitalRead(pin), SLAVE_CMD_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_DIGWRITE:
        pin  = spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_READY);
        data = spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_READY);
        if (pin >= 0 && data >= 0)
          digitalWrite(pin, data &  HIGH);
        break;
      case SLAVE_CMD_ANAREAD:
        pin  = spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_BUSY);
        data = analogRead(pin);
        if (spiWriteByte(data >> 8, SLAVE_DATA_TIMEOUT) >= 0)
          spiWriteByte(data, SLAVE_CMD_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_PWMWRITE:
        pin  = spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_READY);
        data = spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_READY);
        if (pin >= 0 && data >= 0)
          analogWrite(pin, data);
        break;
      case SLAVE_CMD_SERMODE:
        break;
      case SLAVE_CMD_SERAVAIL:
        data = Serial.available();
        spiWriteByte(data, SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SERREAD:
        data = Serial.read();
        spiWriteByte(data, SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SERWRITE:
        data = spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_READY);
        if (data >= 0)
          Serial.write(data);
        break;
      case SLAVE_CMD_BUFREAD:
        count = (spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_READY) << 8)
              |  spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_BUSY);
        if (count > MAX_BUF_SIZE)
          count = MAX_BUF_SIZE;
        for (pin = 0; pin < count; pin++)
          if (spiWriteByte(xferBuf[pin], SLAVE_DATA_TIMEOUT) < 0)
            break;
        spiReady();
        break;
      case SLAVE_CMD_BUFWRITE:
        count = (spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_READY) << 8)
              |  spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_READY);
        if (count > MAX_BUF_SIZE)
          count = MAX_BUF_SIZE;
        for (pin = 0; pin < count; pin++)
        {
          data = spiReadByte(SLAVE_DATA_TIMEOUT, SLAVE_READY);
          if (data < 0)
          {
            spiReady();
            break;
          }
          xferBuf[pin] = data;
        }
        if (pin < MAX_BUF_SIZE) // NULL terminate buffer, just for fun
          xferBuf[pin] = 0;
        break;
      case SLAVE_CMD_SDINIT:
        spiBusy();
        if (!sdInit)
          sdInit = sdFat.begin(sdSSpin, SPI_FULL_SPEED);
        sdFat.chdir("/", true);
        sdFat.vwd()->rewind();
        spiWriteByte(sdInit ? SLAVE_READY : SLAVE_ERROR, SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDCHDIR:
        spiWriteByte(sdFat.chdir((char *)xferBuf, true) ? SLAVE_READY : SLAVE_ERROR, SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDMKDIR:
        spiWriteByte(sdFat.mkdir((char *)xferBuf, true) ? SLAVE_READY : SLAVE_ERROR, SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDRMDIR:
        spiWriteByte(sdFat.rmdir((char *)xferBuf) ? SLAVE_READY : SLAVE_ERROR, SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDRM:
        spiWriteByte(sdFat.remove((char *)xferBuf) ? SLAVE_READY : SLAVE_ERROR, SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDREN:
        spiWriteByte(sdFile.rename(sdFat.vwd(), (char *)xferBuf) ? SLAVE_READY : SLAVE_ERROR, SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDOPENNEXT:
        spiBusy();
        count = 0;
        if (sdFile.openNext(sdFat.vwd(), O_READ))
        {
          if (sdFile.getName((char *)xferBuf, 255))
          {
            if (sdFile.isDir())
              strcat((char *)xferBuf, "/");
            count = strlen((char *)xferBuf);
          }
        }
        spiWriteByte(count, SLAVE_DATA_TIMEOUT);
        spiReady();
         break;
      case SLAVE_CMD_SDOPEN:
        data = spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_BUSY);
        if (data == 0)
          data = O_READ;
        spiWriteByte(sdFile.open((char *)xferBuf, data) ? SLAVE_READY : SLAVE_ERROR, SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDCLOSE:
        spiBusy();
        sdFile.close();
        spiReady();
        break;
      case SLAVE_CMD_SDREAD:
        count = (spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_READY) << 8)
              |  spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_BUSY);
        if (count > 0)
        {
          if (count > MAX_BUF_SIZE)
            count = MAX_BUF_SIZE;
          count = sdFile.read(xferBuf, count);
          if (spiWriteByte(count >> 8, SLAVE_DATA_TIMEOUT) >= 0)
            spiWriteByte(count, SLAVE_CMD_TIMEOUT);
        }
        spiReady();
        break;
      case SLAVE_CMD_SDWRITE:
        count = (spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_READY) << 8)
              |  spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_BUSY);
        if (count > 0)
        {
          if (count > MAX_BUF_SIZE)
            count = MAX_BUF_SIZE;
          count = sdFile.write(xferBuf, count);
          if (count < 0)
            count = 0;
          if (spiWriteByte(count >> 8, SLAVE_DATA_TIMEOUT) >= 0)
            spiWriteByte(count, SLAVE_CMD_TIMEOUT);
        }
        spiReady();
        break;
      default:
        break; 
    }
  }
 }
