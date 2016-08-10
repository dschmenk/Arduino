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
const int SPI_ERR_OK      = 0;
const int SPI_ERR_TIMEOUT = -1;
const int SPI_ERR_BADVAL  = -2;
const int SPI_ERR_WAITING = -3;
/*
 * SPI commands
 */
const int SPI_CMD_NOP      = 0;
const int SPI_CMD_ECHO     = 1;
const int SPI_CMD_HW       = 2;

const int SPI_CMD_PINMODE  = 3;
const int SPI_CMD_DIGREAD  = 4;
const int SPI_CMD_DIGWRITE = 5;
const int SPI_CMD_ANAREAD  = 6;
const int SPI_CMD_PWMWRITE = 7;

const int SPI_CMD_SERMODE  = 8;
const int SPI_CMD_SERAVAIL = 9;
const int SPI_CMD_SERREAD  = 10;
const int SPI_CMD_SERWRITE = 11;

/*
 * How log to wait before checking again
 */
const int SPI_CMD_TIMEOUT  = 1000;
const int SPI_DATA_TIMEOUT = 10000;
const int SPI_DELAY_USEC = 10;
/*
 * Arduino models
 */
const int HW_MODEL_UNO    = 0x01;
const int HW_MODEL_MEGA   = 0x02;
/* 
 *  SPI data transfer variables
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
void spiXfer(void)
{
  byte outPort, spiSR;
  byte timeout;
  
  outPort = PORTD & 0x7F;
  spiSR   = spiAvail ? SLAVE_BUSY : spiOutput;
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD   = outPort | (spiSR & 0x80);
  spiSR   = (spiSR << 1) | (PINB & 1);
  if (!spiAvail)
  {
    spiAvail  = true;
    spiInput  = spiSR;
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
    delayMicroseconds(SPI_DELAY_USEC); 
  } while (timeout--);
  return SPI_ERR_TIMEOUT;
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
    delayMicroseconds(SPI_DELAY_USEC); 
  }
  return SPI_ERR_TIMEOUT;
}
void spiBusy(void)
{
  spiOutput = SLAVE_BUSY;
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
  int cmd, pin, data;
  
  cmd = spiReadByte(0, SLAVE_ECHO);
  if (cmd >= 0)
  {
    switch (cmd)
    {
      case SPI_CMD_NOP:
        spiReady();
        break;
      case SPI_CMD_ECHO:
        data = spiReadByte(SPI_CMD_TIMEOUT, SLAVE_READY);
        if (data >= 0)
          spiWriteByte(data, SPI_CMD_TIMEOUT);
        spiReady();
        break;
      case SPI_CMD_HW:
        spiWriteByte(HW_MODEL_UNO , SPI_CMD_TIMEOUT);
        break;
      case SPI_CMD_PINMODE:
        pin  = spiReadByte(SPI_CMD_TIMEOUT, SLAVE_READY);
        data = spiReadByte(SPI_CMD_TIMEOUT, SLAVE_READY);
        if (pin >=0 && data >= 0)
          pinMode(pin, data);
        break;
      case SPI_CMD_DIGREAD:
        pin  = spiReadByte(SPI_CMD_TIMEOUT, SLAVE_BUSY);
        spiWriteByte(digitalRead(pin), SPI_CMD_TIMEOUT);
        spiReady();
        break;
      case SPI_CMD_DIGWRITE:
        pin  = spiReadByte(SPI_CMD_TIMEOUT, SLAVE_READY);
        data = spiReadByte(SPI_CMD_TIMEOUT, SLAVE_READY);
        if (pin >= 0 && data >= 0)
          digitalWrite(pin, data &  HIGH);
        break;
      case SPI_CMD_ANAREAD:
        pin  = spiReadByte(SPI_CMD_TIMEOUT, SLAVE_BUSY);
        data = analogRead(pin);
        if (spiWriteByte(data >> 8, SPI_DATA_TIMEOUT) >= 0)
          spiWriteByte(data, SPI_CMD_TIMEOUT);
        spiReady();
        break;
      case SPI_CMD_PWMWRITE:
        pin  = spiReadByte(SPI_CMD_TIMEOUT, SLAVE_READY);
        data = spiReadByte(SPI_CMD_TIMEOUT, SLAVE_READY);
        if (pin >= 0 && data >= 0)
          analogWrite(pin, data);
        break;
      case SPI_CMD_SERMODE:
        break;
      case SPI_CMD_SERAVAIL:
        data = Serial.available();
        spiWriteByte(data, SPI_DATA_TIMEOUT);
        break;
      case SPI_CMD_SERREAD:
        data = Serial.read();
        spiWriteByte(data, SPI_DATA_TIMEOUT);
        break;
      case SPI_CMD_SERWRITE:
        data = spiReadByte(SPI_CMD_TIMEOUT, SLAVE_READY);
        if (data >= 0)
          Serial.write(data);
        break;
      default:
        break; 
    }
  }
}
