#include <Esplora.h>

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
}

void loop()
{
  int pin, val;
  int red, green, blue;
  
  if (Serial.available())
  {
    switch (Serial.read())
    {
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
            Serial.print(Esplora.readJoystickY());Serial.print(",");
            Serial.println(Esplora.readJoystickSwitch());
            break;
          case 'B': // Read button pad
          case 'b':
            Serial.print(Esplora.readButton(1));Serial.print(",");
            Serial.print(Esplora.readButton(2));Serial.print(",");
            Serial.print(Esplora.readButton(3));Serial.print(",");
            Serial.println(Esplora.readButton(4));
            break;
          case 'P': // Read potentiometer
          case 'p':
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
        if (Serial.read() != '=')
          break;
        red = Serial.parseInt();
        if (Serial.read() != ',')
        {
          green = red;
          blue  = red;
        }
        else
        {
          green = Serial.parseInt();
          if (Serial.read() == ',')
            blue = Serial.parseInt();
          else
            blue = 0;
        }
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
      case '\n':
        break;
      default:
        Serial.println('?');
    }
  }
}

