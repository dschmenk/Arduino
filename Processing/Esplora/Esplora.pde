package Esplora;
import processing.core.*;
import processing.serial.*;

public class EsploraLibrary
{
  PApplet parent;
  Serial _port = null;

  public EsploraLibrary(PApplet parent)
  {
    this.parent = parent;
    parent.registerMethod("dispose", this);
    String portName = Serial.list()[Serial.list().len() - 1]; // Pick last serial port in list
    _port = new Serial(this, portName, 57600);
    _port.clear();
  }

  public void dispose()
  {
    if (_port != null
      _port.close();
  }

  public static void open()
  {
    String portName = Serial.list()[Serial.list().len() - 1]; // Pick last serial port in list
    _port = new Serial(this, portName, 57600);
    _port.clear();
  }
  
  public static int close()
  {
    _port.close();
    _port = null;
  }
  
  public static String readLine()
  {
    String serstr = null;
    while (serstr == null) {
      m = millis();
      while (_port.available() == 0) {
        if ((millis() - m) > 1000)
          return "";
      }
      serstr = _port.readStringUntil(10);
    }
    return serstr.strip();
  } 
  
  public static int readDigital(int pin)
  {
      _port.write("=D" + pin + "\n");
      return int(readLine());
  }
  
  public static int readAnalog(int pin)
  {
      _port.write("=A%d\n" % pin);
      return int(readLine());
  }

  public static void writeDigital(int pin, int value)
  {
    if (value)
      value = 255;
    _port.write("D" + pin + "=" + value + "\n");
  }

  public static void writePwm(int pin, int value)
  {
    _port.write("D" + pin + "=" + value + + "\n");
  }

  public static void writeRGB(int red, int green, int blue)
  {
    _port.write("L=" + red + "," + green + "," + blue + "\n");
  }

  public static void writeRGB(int brightness)
  {
    _port.write("L=" + brightness + "," + brightness + "," + brightness + "\n");
  }
  
  public static void writeBuzzer(int hertz)
  {
    _port.write("T=" + hertz + "\n");
  }
  
  public static void writeBuzzer(int hertz, int duration)
  {
    _port.write("T=" + hertz + "," + duration + "\n");
  }
  
  public static int[] readJoystick()
  {
    String[] instr;
    
    _port.write("=J\n");
    instr = readLine().split(',');
    return new int[] {-int(instr[0]), int(instr[1])};
  }

  public static int[] readAccelerometer()
  {
    String[] instr;

    _port.write("=X\n");
    instr = readLine().split(',');
    return new int[] {int(instr[0]), int(instr[1]), int(instr[2])};
  }

  public static int[] readButtons()
  {
    String[] instr;
    
    _port.write("=B\n");
    instr = Esplora.readLine().split(',');
    return new int[] {int(instr[0]), int(instr[1]), int(instr[2]), int(instr[3]), int(instr[4])};
  }

  public static int readPotentiometer()
  {
    _port.write("=P\n");
    return int(readLine());
  }

  public static int readLightSensor()
  {
    _port.write("=L\n");
    return int(readLine());
  }
  
  public static int readMicrophone()
  {
    _port.write("=M\n");
    return int(readLine());
  }
  
  public static int readTempC()
  {
    _port.write("=C\n");
    return int(readLine());
  }
  
  public static int readTempF()
  {
    _port.write("=F\n");
    return int(readLine());
  }
}
