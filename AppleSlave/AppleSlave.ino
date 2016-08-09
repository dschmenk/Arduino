/*
 * BIt bang SPI slave mode for Apple II
 */
const int SSpin   = 3;
const int SCKpin  = 6; // PORTD.6 for Uno
const int MISOpin = 7; // PORTD.7 for Uno
const int MOSIpin = 8; // PORTB.0 for Uno

void setup(void)
{
  pinMode(SCKpin,  INPUT);
  pinMode(SSpin,   INPUT);
  pinMode(MOSIpin, INPUT);
  pinMode(MISOpin, OUTPUT);
  digitalWrite(MISOpin, LOW) ;
  attachInterrupt(digitalPinToInterrupt(SSpin), spiXfer, FALLING);
  Serial.begin(9600);
  Serial.print("Starting...\n");
}
//
// Shift in/out serial data
//
volatile byte spiData = 0;
volatile byte spiAvail = false;
void spiXfer(void)
{
  byte outPort;
  byte timeout;

  if (spiAvail)
    return;
  outPort = PORTD & 0x7F;
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD = outPort | (spiData & 0x80);
  spiData = (spiData << 1) | (PINB & 1);
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD = outPort | (spiData & 0x80);
  spiData = (spiData << 1) | (PINB & 1);
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD = outPort | (spiData & 0x80);
  spiData = (spiData << 1) | (PINB & 1);
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD = outPort | (spiData & 0x80);
  spiData = (spiData << 1) | (PINB & 1);
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD = outPort | (spiData & 0x80);
  spiData = (spiData << 1) | (PINB & 1);
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD = outPort | (spiData & 0x80);
  spiData = (spiData << 1) | (PINB & 1);
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD = outPort | (spiData & 0x80);
  spiData = (spiData << 1) | (PINB & 1);
  timeout = 0xFF; while (PIND & 0x40 && timeout--);
  PORTD = outPort | (spiData & 0x80);
  spiData = (spiData << 1) | (PINB & 1);
  spiAvail = true;
}
/*
 * SPI slave to Apple II
 */
void loop(void)
{
  if (spiAvail)
  {
    Serial.write (spiData);
    spiAvail = false;
  }
}
