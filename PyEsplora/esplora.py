import serial

class Esplora:
    _port = {}

    @staticmethod
    def open(portname = "/dev/tty.usbmodem411"):
        Esplora._port = serial.Serial(portname, 57600, timeout = 1)
    
    @staticmethod
    def close():
        Esplora._port.close()

    @staticmethod
    def readDigital(pin):
        Esplora._port.write("=D%d\n" % pin)
        return int(Esplora._port.readline().rstrip())

    @staticmethod
    def readAnalog(pin):
        Esplora._port.write("=A%d\n" % pin)
        return int(Esplora._port.readline().rstrip())

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
        instr = Esplora._port.readline().rstrip().split(",")
        return [int(instr[0]), int(instr[1])]

    @staticmethod
    def readAccelerometer():
        Esplora._port.write("=X\n")
        instr = Esplora._port.readline().rstrip().split(",")
        return [int(instr[0]), int(instr[1]), int(instr[2])]

    @staticmethod
    def readButtons():
        Esplora._port.write("=B\n")
        instr = Esplora._port.readline().rstrip().split(",")
        return [int(instr[0]), int(instr[1]), int(instr[2]), int(instr[3]), int(instr[4])]

    @staticmethod
    def readPotentiometer():
        Esplora._port.write("=P\n")
        return int(Esplora._port.readline().rstrip())

    @staticmethod
    def readLightSensor():
        Esplora._port.write("=L\n")
        return int(Esplora._port.readline().rstrip())

    @staticmethod
    def readMicrophone():
        Esplora._port.write("=M\n")
        return int(Esplora._port.readline().rstrip())

    @staticmethod
    def readTempC():
        Esplora._port.write("=C\n")
        return int(Esplora._port.readline().rstrip())

    @staticmethod
    def readTempF():
        Esplora._port.write("=F\n")
        return int(Esplora._port.readline().rstrip())
