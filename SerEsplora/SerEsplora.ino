#include <Esplora.h>
#include <TFT.h>
#include <SPI.h>

/*
 One time initialization.
 */
void setup()
{
  // initialize serial:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  // initialize TFT screen
  EsploraTFT.begin();
}

void readRGB(byte *red, byte *green, byte *blue)
{
  if (Serial.read() != '=')
    return;
  *red = Serial.parseInt();
  if (Serial.read() != ',')
  {
    *green = *red;
    *blue  = *red;
  }
  else
  {
    *green = Serial.parseInt();
    if (Serial.read() == ',')
      *blue = Serial.parseInt();
    else
      *blue = 0;
  }
}

void loop()
{
  int pin, val, x1, y1, x2, y2;
  byte red, green, blue;
  char string[40];
  
  if (Serial.available())
  {
    switch (Serial.read())
    {
      case '!': // Synchronize
        Serial.println("!");
        break;
      case '=': // Read a value
        switch (Serial.read())
        {
          case 'D': // Read digital pin
          case 'd':
            pin = Serial.parseInt();
            pinMode(pin, INPUT);
            Serial.println(digitalRead(pin));
            break;
          case 'A': // Read analog pin
          case 'a':
            pin = Serial.parseInt() + A0;
            pinMode(pin, INPUT);
            Serial.println(analogRead(pin));
            break;
          case 'X': // Read accelerometer
          case 'x':
            Serial.print(Esplora.readAccelerometer(X_AXIS));Serial.print(",");
            Serial.print(Esplora.readAccelerometer(Y_AXIS));Serial.print(",");
            Serial.println(Esplora.readAccelerometer(Z_AXIS));
            break;
          case 'J': // Read joystick
          case 'j':
            Serial.print(Esplora.readJoystickX());Serial.print(",");
            Serial.println(Esplora.readJoystickY());
            break;
          case 'B': // Read button pad
          case 'b':
            Serial.print(Esplora.readButton(1));Serial.print(",");
            Serial.print(Esplora.readButton(2));Serial.print(",");
            Serial.print(Esplora.readButton(3));Serial.print(",");
            Serial.print(Esplora.readButton(4));Serial.print(",");
            Serial.println(Esplora.readJoystickSwitch());
            break;
          case 'S': // Read slider
          case 's':
            Serial.println(Esplora.readSlider());
            break;
          case 'L': // Read light sensor
          case 'l':
            Serial.println(Esplora.readLightSensor());
            break;
          case 'M': // Read microphone
          case 'm':
            Serial.println(Esplora.readMicrophone());
            break;
          case 'C': // Read temperature in Celcius
          case 'c':
            Serial.println(Esplora.readTemperature(DEGREES_C));
            break;
          case 'F': // Read temperature in Celcius
          case 'f':
            Serial.println(Esplora.readTemperature(DEGREES_F));
            break;
          case 'Z': // Read TFT screen size
          case 'z':
            Serial.print(EsploraTFT.width());Serial.print(",");
            Serial.println(EsploraTFT.height());
            break;
        }
        break;
      case 'D': // Write digital pin
      case 'd':
        pin = Serial.parseInt();
        switch (Serial.read())
        {
          case '=': // Binary
            val = Serial.parseInt() ? HIGH : LOW;
            pinMode(pin, OUTPUT);
            digitalWrite(pin, val);
            break;
          case '~': // PWM
            val = Serial.parseInt();
            pinMode(pin, OUTPUT);
            analogWrite(pin, val);
            break;
        }
        break;
      case 'A': // Write analog pin
      case 'a':
        pin = Serial.parseInt() + A0;
        if (Serial.read() != '=')
          break;
        val = Serial.parseInt();
        pinMode(pin, OUTPUT);
        digitalWrite(pin, val);
        break;
      case 'L': // Write RGB LED
      case 'l':
        readRGB(&red, &green, &blue);
        Esplora.writeRGB(red, green, blue);
        break;
      case 'T': // Write tone
      case 't':
        if (Serial.read() != '=')
          break;
        val = Serial.parseInt();
        if (Serial.read() == ',')
          Esplora.tone(val, Serial.parseInt());
        else
          Esplora.tone(val);
        break;
      case 'S': // call TFT screen functions
      case 's':
        switch (Serial.read())
        {
          case 'B': // background
          case 'b':
            readRGB(&red, &green, &blue);
            EsploraTFT.background(blue, green, red);
            break;
          case 'F': // fill color
          case 'f':
            readRGB(&red, &green, &blue);
            EsploraTFT.fill(blue, green, red);
            break;
          case 'S': // stroke color
          case 's':
            readRGB(&red, &green, &blue);
            EsploraTFT.stroke(blue, green, red);
            break;
          case 'N': // no stroke/fill
          case 'n':
            switch (Serial.read())
            {
              case 'S': // no stroke
              case 's':
                EsploraTFT.noStroke();
                break;
              case 'F': // no fill
              case 'f':
                EsploraTFT.noFill();
                break;
            }
            break;
          case 'R': // rect
          case 'r':
            if (Serial.read() != '=')
              break;
            x1 = Serial.parseInt();
            if (Serial.read() != ',')
              break;
            y1 = Serial.parseInt();
            if (Serial.read() != ',')
              break;
            x2 = Serial.parseInt();
            if (Serial.read() != ',')
              break;
            y2 = Serial.parseInt();
            EsploraTFT.rect(x1, y1, x2, y2);
            break;
          case 'L': // line
          case 'l':
            if (Serial.read() != '=')
              break;
            x1 = Serial.parseInt();
            if (Serial.read() != ',')
              break;
            y1 = Serial.parseInt();
            if (Serial.read() != ',')
              break;
            x2 = Serial.parseInt();
            if (Serial.read() != ',')
              break;
            y2 = Serial.parseInt();
            EsploraTFT.line(x1, y1, x2, y2);
            break;
          case 'C': // circle
          case 'c':
            if (Serial.read() != '=')
              break;
            x1 = Serial.parseInt();
            if (Serial.read() != ',')
              break;
            y1 = Serial.parseInt();
            if (Serial.read() != ',')
              break;
            x2 = Serial.parseInt();
            EsploraTFT.circle(x1, y1, x2);
            break;
          case 'P': // point
          case 'p':
            if (Serial.read() != '=')
              break;
            x1 = Serial.parseInt();
            if (Serial.read() != ',')
              break;
            y1 = Serial.parseInt();
            if (Serial.read() != ',')
              break;
            EsploraTFT.point(x1, y1);
            break;
          case 'T': // text
          case 't':
            if (Serial.read() != '=')
              break;
            if (Serial.read() != '\"')
              break;
            val = 0;
            while ((pin = Serial.read()) != '\"')
            {
              string[val++] = pin;
            }
            string[val++] = '\0';
            if (Serial.read() != ',')
              break;
            x1 = Serial.parseInt();
            if (Serial.read() != ',')
              break;
            y1 = Serial.parseInt();
            if (Serial.read() != ',')
              break;
            EsploraTFT.text(string, x1, y1);
            break;
          case 'Z': // text size
          case 'z':
            if (Serial.read() != '=')
              break;
            val = Serial.parseInt();
            EsploraTFT.setTextSize(val);
            break;
          default:
            Serial.println('?');
        }
        break;
      case '\n':
        break;
      default:
        Serial.println('?');
    }
  }
}

