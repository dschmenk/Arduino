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
const int SLAVE_CMD_CWD      = 15;
const int SLAVE_CMD_SDCHDIR  = 16;
const int SLAVE_CMD_SDMKDIR  = 17;
const int SLAVE_CMD_SDRMDIR  = 18;
const int SLAVE_CMD_SDRM     = 19;
const int SLAVE_CMD_SDREN    = 20;
const int SLAVE_CMD_SDOPEN   = 21;
const int SLAVE_CMD_SDOPENFIRST=22;
const int SLAVE_CMD_SDOPENNEXT=23;
const int SLAVE_CMD_SDCLOSE  = 24;
const int SLAVE_CMD_SDREAD   = 25;
const int SLAVE_CMD_SDWRITE  = 26;
const int SLAVE_CMD_SDSYNC   = 27;
const int SLAVE_CMD_SDREWIND = 28;
const int SLAVE_CMD_SDSEEKOFS= 29;
const int SLAVE_CMD_SDSEEKSET= 30;
const int SLAVE_CMD_SDSEEKEOF= 31;
const int SLAVE_CMD_SDPOS    = 32;
const int SLAVE_CMD_SDSIZE   = 33;
const int SLAVE_CMD_SDTRUNC  = 34;
const int SLAVE_CMD_SDISDIR  = 35;
const int SLAVE_CMD_SDISFILE = 36;
const int SLAVE_CMD_SDEXISTS = 37;
/*
 * How long to wait in usec before timing out
 */
const int SLAVE_CMD_TIMEOUT  = 10000;
const int SLAVE_DATA_TIMEOUT = 100000;
const int SLAVE_DELAY        = 0xFF;
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
volatile  byte spiOutput = SLAVE_BUSY;
/*
 * Initialize it all
 */
void setup(void)
{
  pinMode(SCKpin,  INPUT);
  pinMode(SSpin,   INPUT);
  pinMode(MOSIpin, INPUT);
  pinMode(MISOpin, OUTPUT);
  digitalWrite(MISOpin, LOW);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(SSpin), spiXfer, FALLING);
  //EICRA = 0x0A; // Falling edge triggered IRQ
  //EIMSK = 0x02; // INT1 enabled
  spiReady();
}
//
// Shift in/out serial data
//
const int    SLAVE_CLK_TIMEOUT = 0xFF;
#define SCLK_FALL for(timeout=SLAVE_CLK_TIMEOUT;(PIND&0x40)&&timeout--;)
#define SCLK_RISE for(timeout=SLAVE_CLK_TIMEOUT;!(PIND&0x40)&&timeout--;)
//ISR(INT1_vect) {spiXfer();}
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
  for (timeout = 0xFF;!(PIND&0x08) && timeout--;); // wait for SS de-assert
}
/*
 * SPI slave to Apple II
 */
int spiReadByte(unsigned long timeout, int nextOut)
{
  unsigned long time = micros();
  byte readByte;
  
  do {
    if (spiAvail)
    {
      spiOutput = nextOut;
      readByte  = spiInput;
      spiAvail  = false;
      return readByte;
    }
  } while (micros() - time < timeout);
  return SLAVE_ERR_TIMEOUT;
}
int spiWriteByte(byte val, unsigned long timeout)
{
  unsigned long time = micros();
  byte readByte;
  
  noInterrupts();
  spiOutput = val;
  spiAvail  = false;
  interrupts();
  do
  {
    if (spiAvail)
    {
      readByte = spiInput;
      spiAvail = false;
      return readByte;
    }
  } while (micros() - time < timeout);
  return SLAVE_ERR_TIMEOUT;
}
void spiBusy(void)
{
  spiOutput = SLAVE_BUSY;
  PORTD |= 0x80; // Set output high so master will read FF if slave busy w/ ints off
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
  byte *pBuf;
  
  cmd = spiReadByte(0, SLAVE_READY);
  if (cmd >= 0)
  {
    switch (cmd)
    {
      case SLAVE_CMD_NOP:
        spiBusy();
        Serial.println("Cmd: NOP");
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
        /*
         * Uno goes out to lunch for awhile during back-to-back xfers. Not sure why
         */
#if 0
        for (pBuf = xferBuf; count--; pBuf++)
          if (spiWriteByte(*pBuf, SLAVE_DATA_TIMEOUT) < 0)
            break;
#else
        noInterrupts();
        for (pBuf = xferBuf; count--; pBuf++)
        {
          unsigned int timeout;
          
          spiOutput = *pBuf;
          spiAvail  = false;
          for (timeout = 0xFFFF;(PIND&0x08) && timeout--;); // wait for SS assert
          spiXfer();
        }
        EIFR = 0x02; // clear pending interrupt
        interrupts();
#endif
        spiReady();
        break;
      case SLAVE_CMD_BUFWRITE:
        count = (spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_READY) << 8)
              |  spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_BUSY);
        if (count >= MAX_BUF_SIZE)
          count = MAX_BUF_SIZE;
        else
          xferBuf[count] = 0; // NULL terminate xfer, just for fun
        /*
         * Uno goes out to lunch for awhile during back-to-back xfers. Not sure why
         */
#if 0
        for (pBuf = xferBuf; count--; pBuf++)
        {
          data = spiReadByte(SLAVE_DATA_TIMEOUT, SLAVE_READY);
          if (data < 0)
          {
            spiReady();
            break;
          }
          *pBuf = data;
        }
#else
        noInterrupts();
        for (pBuf = xferBuf; count--; pBuf++)
        {
          unsigned int timeout;
          
          spiOutput = SLAVE_READY;
          spiAvail  = false;
          for (timeout = 0xFFFF;(PIND&0x08) && timeout--;); // wait for SS assert
          spiXfer();
          *pBuf = spiInput;
        }
        spiOutput = SLAVE_READY;
        spiAvail  = false;
        EIFR = 0x02; // clear pending interrupt
        interrupts();
#endif
        break;
      case SLAVE_CMD_SDINIT:
        spiBusy();
        if (!sdInit)
          sdInit = sdFat.begin(sdSSpin, SPI_FULL_SPEED);
        sdFile.close();          
        sdFat.chdir("/", true);
        sdFat.vwd()->rewind();
        spiWriteByte(sdInit ? SLAVE_READY : SLAVE_ERROR, SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_CWD:
        spiBusy();
        count = 0;
        if (sdFat.vwd()->getName((char *)xferBuf, 255))
          count = strlen((char *)xferBuf);
        spiWriteByte(count, SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDCHDIR:
        spiBusy();
        spiWriteByte(sdFat.chdir((char *)xferBuf, true), SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDMKDIR:
        spiBusy();
        spiWriteByte(sdFat.mkdir((char *)xferBuf, true), SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDRMDIR:
        spiBusy();
        spiWriteByte(sdFat.rmdir((char *)xferBuf), SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDRM:
        spiBusy();
        spiWriteByte(sdFile.remove(), SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDREN:
        spiBusy();
        spiWriteByte(sdFile.rename(sdFat.vwd(), (char *)xferBuf), SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDOPENFIRST:
        spiBusy();
        sdFat.vwd()->rewind();
      case SLAVE_CMD_SDOPENNEXT:
        spiBusy();
        count = 0;
         if (sdFile.openNext(sdFat.vwd(), O_READ) && sdFile.getName((char *)xferBuf, 255))
          count = strlen((char *)xferBuf);
        spiWriteByte(count, SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDOPEN:
        data = spiReadByte(SLAVE_CMD_TIMEOUT, SLAVE_BUSY);
        if (data == 0)
          data = O_READ;
        spiWriteByte(sdFile.open((char *)xferBuf, data), SLAVE_DATA_TIMEOUT);
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
      case SLAVE_CMD_SDSYNC:
      case SLAVE_CMD_SDREWIND:
      case SLAVE_CMD_SDSEEKOFS:
      case SLAVE_CMD_SDSEEKSET:
      case SLAVE_CMD_SDSEEKEOF:
      case SLAVE_CMD_SDPOS:
      case SLAVE_CMD_SDSIZE:
      case SLAVE_CMD_SDTRUNC:
      break;
      case SLAVE_CMD_SDISDIR:
        spiWriteByte(sdFile.isDir(), SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDISFILE:
        spiWriteByte(sdFile.isFile(), SLAVE_DATA_TIMEOUT);
        spiReady();
        break;
      case SLAVE_CMD_SDEXISTS:
        break;
      default:
        Serial.print("Huh? Cmd:");Serial.println(cmd);
        break; 
    }
  }
 }
