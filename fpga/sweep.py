import serial
import serial.tools.list_ports
import time
import sys
import matplotlib.pyplot as plt
import random

VECTORS=100
PROGBAR=50
KEYWORD="Lattice FTUSB Interface Cable"

class uart:
    def get(self):
        rx = int.from_bytes(self.ser.read(1), byteorder='big')
        return rx

    def swap(self, d):
        self.ser.write(d.to_bytes(1, byteorder='big'))
        r = int.from_bytes(self.ser.read(1), byteorder='big')
        if(r != d):
            print("Error")

    def create(self,port,baud):
        self.ser = serial.Serial(
            port=port,
            baudrate=baud,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS
        )
        self.ser.flushInput()
        self.ser.flushOutput()
        print(f"Baud: {baud}")

    def destory(self):
        self.ser.close()

    def search(self):
        ports = serial.tools.list_ports.comports()
        for port, desc, _ in sorted(ports):
            if KEYWORD in desc:
                print(f"Found: {port} to contain {KEYWORD}")
                return str(port)
        print("Error: No port found")
        sys.exit(1)

def main():
    u = uart()
    bauds = [9600,115200]
    p = u.search()
    for b in bauds:
        old_bar = 0
        u.create(p,b)
        for i in range(VECTORS):
            u.swap(random.randrange(0x00,0xFF))
            new = float(1+i)/float(VECTORS)
            percent = int(new * 100)
            bar = int(new * PROGBAR)
            if(bar > old_bar):
                old_bar = bar
                s = f"\r{percent:03}%: |"
                for j in range(PROGBAR):
                    if(j < bar):
                        s += '#'
                    else:
                        s += ' '
                s += "|"
                print(s,end="")
        u.destory()
        print("\n")

if "__main__" == __name__:
    main()

