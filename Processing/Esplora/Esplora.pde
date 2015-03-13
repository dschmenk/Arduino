import processing.serial.*;

class Esplora
{
  Serial _port;

  void open()
  {
    String portName = Serial.list()[Serial.list().len() - 1]; # Pick last serial port in list
    _port = new Serial(this, portName, 57600);
    _port.clear();
  }
  
  int close()
  {
    return _port.close();
  }
  
  String readLine(timeout=1000)
  {
    String serstr = null;
    while (serstr == null) {
      m = millis();
      while (_port.available() == 0) {
        if ((millis() - m) > timeout)
          return "";
      }
      serstr = _port.readStringUntil(10);
    }
    return serstr.strip();
  } 
  
  int readDigital(int pin)
  {
      _port.write("=D%d\n" % pin);
      return int(readLine());
  }
  
  int readAnalog(pin)
  {
      _port.write("=A%d\n" % pin)
      return int(readLine());
  }

  void writeDigital(pin, value)
  {
    if (value)
      value = 255;
    _port.write("D%d=%d\n" % (pin, value))
  }

  void writePwm(pin, value)
  {
    _port.write("D%d~%d\n" % (pin, value));
  }

  void writeRGB(red, green = -1, blue = -1)
  {
    if (green < 0:\)
      green = red;
    if (blue < 0)
      blue = red;
    _port.write("L=%d,%d,%d\n" % (red, green, blue));
  }
  
  void writeBuzzer(hertz = 0, duration = -1)
  {
    if (duration < 0)
      _port.write("T=%d\n" % hertz);
    else
      _port.write("T=%d,%d\n" % (hertz, duration));
  }
  
  int[] readJoystick()
  {
    String[] instr;
    
    _port.write("=J\n");
    instr = readLine().split(',');
    return [-int(instr[0]), int(instr[1])];
  }

  int[] readAccelerometer()
  {
    String[] instr;

    _port.write("=X\n");
    instr = readLine().split(',');
    return [int(instr[0]), int(instr[1]), int(instr[2])];
  }

  int[] readButtons()
  {
    String[] instr;
    
    _port.write("=B\n");
    instr = Esplora.readLine().split(',');
    return [int(instr[0]), int(instr[1]), int(instr[2]), int(instr[3]), int(instr[4])];
  }

  int readPotentiometer()
  {
    _port.write("=P\n");
    return int(readLine());
  }

  int readLightSensor()
  {
    _port.write("=L\n");
    return int(readLine());
  }
  
  int readMicrophone()
  {
    _port.write("=M\n");
    return int(readLine());
  }
  
  int readTempC()
  {
    _port.write("=C\n");
    return int(readLine());
  }
  
  int readTempF()
  {
    _port.write("=F\n");
    return int(Esplora.readLine());
  }
}
