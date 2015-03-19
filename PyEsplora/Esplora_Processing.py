class Esplora:
    _port = {}

    @staticmethod
    def open(Serial):
        portName = Serial.list()[len(Serial.list()) - 1] # Pick last serial port in list
        Esplora._port = Serial(this, portName, 57600)
        Esplora._port.clear()
        Esplora._port.write("!\n")
        Esplora._port.readline(5000)

    @staticmethod
    def close():
        Esplora._port.close()

    @staticmethod
    def readLine(timeout=1000):
        serstr = None
        while serstr == None:
            m = millis()
            while Esplora._port.available() == 0:
                if (millis() - m) > timeout:
                    return ""
            serstr = Esplora._port.readStringUntil(10)
        return serstr.strip()
    
    @staticmethod
    def readDigital(pin):
        Esplora._port.write("=D%d\n" % pin)
        return int(Esplora.readLine())

    @staticmethod
    def readAnalog(pin):
        Esplora._port.write("=A%d\n" % pin)
        return int(Esplora.readLine())

    @staticmethod
    def writeDigital(pin, value):
        if value:
            value = 255
        Esplora._port.write("D%d=%d\n" % (pin, value))

    @staticmethod
    def writePwm(pin, value):
        Esplora._port.write("D%d~%d\n" % (pin, value))

    @staticmethod
    def writeRGB(red, green = -1, blue = -1):
        if green < 0:
            green = red
        if blue < 0:
            blue = red
        Esplora._port.write("L=%d,%d,%d\n" % (red, green, blue))

    @staticmethod
    def writeBuzzer(hertz = 0, duration = -1):
        if duration < 0:
            Esplora._port.write("T=%d\n" % hertz)
        else:
            Esplora._port.write("T=%d,%d\n" % (hertz, duration))

    @staticmethod
    def readJoystick():
        Esplora._port.write("=J\n")
        instr = Esplora.readLine().split(',')
        return [-int(instr[0]), int(instr[1])]

    @staticmethod
    def readAccelerometer():
        Esplora._port.write("=X\n")
        instr = Esplora.readLine().split(',')
        return [int(instr[0]), int(instr[1]), int(instr[2])]

    @staticmethod
    def readButtons():
        Esplora._port.write("=B\n")
        instr = Esplora.readLine().split(',')
        return [int(instr[0]), int(instr[1]), int(instr[2]), int(instr[3]), int(instr[4])]

    @staticmethod
    def readSlider():
        Esplora._port.write("=S\n")
        return int(Esplora.readLine())

    @staticmethod
    def readLightSensor():
        Esplora._port.write("=L\n")
        return int(Esplora.readLine())

    @staticmethod
    def readMicrophone():
        Esplora._port.write("=M\n")
        return int(Esplora.readLine())

    @staticmethod
    def readTempC():
        Esplora._port.write("=C\n")
        return int(Esplora.readLine())

    @staticmethod
    def readTempF():
        Esplora._port.write("=F\n")
        return int(Esplora.readLine())

    @staticmethod
    def tftSize()
        Esplora._port.write("=Z\n")
        instr = Esplora._port.readline().rstrip().split(",")
        return [int(instr[0]), int(instr[1])]
    
    @staticmethod
    def tftBackground(red, green = -1, blue = -1):
        if green < 0:
            green = red
        if blue < 0:
            blue = red
        Esplora._port.write("SB=%d,%d,%d\n" % (red, green, blue))
        Esplora._port.write("!\n")
        Esplora._port.readline()

    @staticmethod
    def tftFill(red, green = -1, blue = -1):
        if green < 0:
            green = red
        if blue < 0:
            blue = red
        Esplora._port.write("SF=%d,%d,%d\n" % (red, green, blue))
        Esplora._port.write("!\n")
        Esplora._port.readline()

    @staticmethod
    def tftStroke(red, green = -1, blue = -1):
        if green < 0:
            green = red
        if blue < 0:
            blue = red
        Esplora._port.write("SS=%d,%d,%d\n" % (red, green, blue))
        Esplora._port.write("!\n")
        Esplora._port.readline()

    @staticmethod
    def tftNoFill():
        Esplora._port.write("SNF\n")
        Esplora._port.write("!\n")
        Esplora._port.readline()

    @staticmethod
    def tftNoStroke():
        Esplora._port.write("SNS\n")
        Esplora._port.write("!\n")
        Esplora._port.readline()

    @staticmethod
    def tftRect(xpos, ypos, width, height):
        Esplora._port.write("SR=%d,%d,%d,%d\n" % (xpos, ypos, width, height))
        Esplora._port.write("!\n")
        Esplora._port.readline()

    @staticmethod
    def tftLine(x1, y1, x2, y2):
        Esplora._port.write("SL=%d,%d,%d,%d\n" % (x1, y1, x2, y2))
        Esplora._port.write("!\n")
        Esplora._port.readline()

    @staticmethod
    def tftCircle(xpos, ypos, radius):
        Esplora._port.write("SC=%d,%d,%d\n" % (xpos, ypos, radius))
        Esplora._port.write("!\n")
        Esplora._port.readline()

    @staticmethod
    def tftText(string, xpos, ypos):
        Esplora._port.write('ST="%s",%d,%d\n' % (string, xpos, ypos))
        Esplora._port.write("!\n")
        Esplora._port.readline()

    @staticmethod
    def tftTextSize(size):
        Esplora._port.write('SZ=%d\n' % (size))
        Esplora._port.write("!\n")
        Esplora._port.readline()
