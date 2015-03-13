class Esplora:
    _port = {}

    @staticmethod
    def open(Serial):
        portName = Serial.list()[len(Serial.list()) - 1] # Pick last serial port in list
        Esplora._port = Serial(this, portName, 57600)
        Esplora._port.clear()

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
    def readPotentiometer():
        Esplora._port.write("=P\n")
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
