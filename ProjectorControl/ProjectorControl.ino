/*
 * Saint Francis Projector Controller
 *
#define  SERIAL_OUTPUT
 */
#define  IR_RECV       5
#define  IR_LED        6
#define  STATUS_LED    13
#define  DOWN_BUTTON   2
#define  UP_BUTTON     3
#define  SOURCE_BUTTON 4
#define  UP_RELAY      7
#define  DOWN_RELAY    8
#define  LOWER_SECS    15
#define  RAISE_SECS    16

const int  irSpace  = 500;
const int  irOne    = 1500;
const int  irZero   = 500;
const int  irHdrHi  = 9000;
const int  irHdrLo  = 4500;
int irBit[]         = {irZero, irOne};
#define  IR_SLOP      100
#define  IR_TIMEOUT   irHdrHi*2
/*
 * Benq codes
 */
const word benqPre    = 0x000C;
const word benqOn     = 0xF20D;
const word benqOff    = 0x728D;
const word benqSource = 0x20DF;
const word benqComp   = 0x8A75;
const word benqVideo  = 0x4AB5;
const word benqSVideo = 0xF807;
const word benqHDMI1  = 0x1AE5;
const word benqHDMI2  = 0x9A65;
const word benqPC     = 0x827D;
/*
 * Cycle inputs
 */
word benqInputs[] = {benqPC, benqHDMI1};
byte benqSelIn    = 0;
#define  MAX_BENQ_INPUTS  2
/*
 * Screen position
 */
#define SCREEN_DOWN  1
#define  SCREEN_UP   0
byte screenPos = SCREEN_UP;

void setup()
{
#ifdef SERIAL_OUTPUT
  Serial.begin(115200);
  Serial.println("St. Francis Smart Projector Switch");
#endif
  digitalWrite(STATUS_LED, LOW);
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(IR_LED, LOW);
  pinMode(IR_LED, OUTPUT);
  pinMode(IR_RECV, INPUT);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(SOURCE_BUTTON, INPUT_PULLUP);
  digitalWrite(UP_RELAY, LOW);
  digitalWrite(DOWN_RELAY, LOW);
  pinMode(UP_RELAY, OUTPUT);
  pinMode(DOWN_RELAY, OUTPUT);
#if 0  
  int usec = 2;
  noInterrupts();
  while (usec > 0)
  {
    digitalWrite(IR_LED, HIGH);
    delayMicroseconds(1);
    digitalWrite(IR_LED, LOW);
    delayMicroseconds(2);
    usec++;
  }
#endif
}

void irWrite(int usec, byte val)
{
  /*
   * Write 38KHz for useconds
   */
  while (usec > 0)
  {
    digitalWrite(IR_LED, val);
    delayMicroseconds(1);
    digitalWrite(IR_LED, LOW);
    delayMicroseconds(2);
    usec -= 26;
  }
}
/*
 * Send BENQ IR sequence
 */
void benqWriteWord(word val)
{
  for (byte i = 0; i < 16; i++)
  {
    irWrite(irSpace, HIGH);
    irWrite(irBit[val & 0x8000 ? 1 : 0], LOW);
    val <<= 1;
  }
}
void benqSend(word code)
{
  /*
   * Send header
   */
  noInterrupts();
  irWrite(irHdrHi, HIGH);
  irWrite(irHdrLo, LOW);
  /*
   * Send code
   */
  benqWriteWord(benqPre);
  benqWriteWord(code);
  /*
   * Send trailer
   */
  irWrite(irSpace, HIGH);
  interrupts();
}
word benqRecv(void)
{
  word pulse;
  unsigned long bitstream;
  byte valid = true;
#ifdef SERIAL_OUTPUT
  word seq[34];
  byte count = 0;
#endif
  if (pulseIn(IR_RECV, LOW, IR_TIMEOUT) > irHdrHi/2)
  {  
    bitstream = 0;
    while ((pulse = pulseIn(IR_RECV, HIGH, IR_TIMEOUT)))
    {
      if (pulse > (irOne-IR_SLOP) && pulse < (irOne+IR_SLOP))
        bitstream = (bitstream << 1) | 1;
      else if (pulse > (irZero-IR_SLOP) && pulse < (irZero+IR_SLOP))
        bitstream <<= 1;
      else
        valid = false;
#ifdef SERIAL_OUTPUT
      seq[count++] = pulse;
 #endif
    }
#ifdef SERIAL_OUTPUT
    Serial.print("0x"); Serial.println(bitstream, HEX);
    for (byte i = 0; i < count; i++)
         Serial.println(seq[i]);
#endif
    if (valid && (bitstream>>16) == benqPre)
      return (word)bitstream;
  }
  return 0;
}
void screenLower()
{
#ifdef SERIAL_OUTPUT
  Serial.println("Lower Screen...");
#endif
    digitalWrite(DOWN_RELAY, HIGH);
    for (byte i = 0; i < 5; i++)
    {
      benqSend(benqOn);
      delay(100);
    }
    digitalWrite(DOWN_RELAY, LOW);
    screenPos = SCREEN_DOWN;
}

void screenRaise()
{
#ifdef SERIAL_OUTPUT
  Serial.println("Raise Screen...");
#endif
    digitalWrite(UP_RELAY, HIGH);
    for (byte i = 0; i < 5; i++)
    {
      benqSend(benqOff);
      delay(100);
    }
    digitalWrite(UP_RELAY, LOW);
    screenPos = SCREEN_UP;
}

void loop()
{
  int i;
  word benqRepeat;
  
  /*
   * Repeat any IR commands
   */
  if ((benqRepeat = benqRecv()))
    benqSend(benqRepeat);
  /*
   * Check button status
   */
  if (digitalRead(DOWN_BUTTON) == LOW) // Down button pressed
  {
    if (screenPos == SCREEN_UP) // If screen up, then lower it
    {
      screenLower();
      for (i = 0; i < LOWER_SECS; i++)
      {
        digitalWrite(STATUS_LED, HIGH);
        delay(750);
        digitalWrite(STATUS_LED, LOW);
        delay(250);
      }
    }
    else  // Treat down button as source button when screen already down
    { 
      digitalWrite(STATUS_LED, HIGH);
      benqSend(benqOn); // Turn it on in case it went to sleep
      delay(100);
      benqSend(benqOn);
      delay(100);
      benqSend(benqInputs[benqSelIn]);
      delay(100);
      benqSend(benqInputs[benqSelIn]);
      delay(100);
      if (++benqSelIn >= MAX_BENQ_INPUTS)
        benqSelIn = 0;
      digitalWrite(STATUS_LED, LOW);
      while (digitalRead(DOWN_BUTTON) == LOW); // Wait until down button released
    }
  }
  if (digitalRead(UP_BUTTON) == LOW) // Up button pressed
  {
    screenRaise();
    for (i = 0; i < RAISE_SECS; i++)
    {
      digitalWrite(STATUS_LED, HIGH);
      delay(250);
      digitalWrite(STATUS_LED, LOW);
      delay(750);
    }
  }
}
