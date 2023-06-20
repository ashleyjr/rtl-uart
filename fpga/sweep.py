import serial
import serial.tools.list_ports
import time
import sys
import matplotlib.pyplot as plt
import random
import os

VECTORS=1024
BURST=128
PROGBAR=50
KEYWORD="Lattice FTUSB Interface Cable"
CLKS=[24000000,48000000,96000000,192000000]
BAUDS=[115200,128000,256000]

#BAUDS=[9600,14400,19200,38400,57600,115200,128000,256000]

class uart:


    def genVectors(self, length):
        self.tx_ptr = 0
        self.rx_ptr = 0
        self.vectors = bytearray()
        for i in range(length):
            d = random.randrange(0x00,0xFF)
            d = d.to_bytes(1, byteorder='big')
            self.vectors += d

    def get(self):
        rx = int.from_bytes(self.ser.read(1), byteorder='big')
        return rx

    def send(self, length):
        burst = bytearray()
        for i in range(length):
            burst.append(self.vectors[self.tx_ptr])
            self.tx_ptr += 1
        self.ser.write(burst)

    def get(self, length):
        rxs = self.ser.read(length)
        error = False
        for rx in rxs:
            d = self.vectors[self.rx_ptr]
            if(rx != d):
                print(f"Error: Pos={self.rx_ptr}, Epected={d}, Got={rx}")
                eror = True
            self.rx_ptr += 1
        if(len(rxs) != length):
            print(f"Error: Timeout - Expected #{length} bytes, Got #{len(rxs)} bytes")
            error = True
        if(error):
            self.destory()
            sys.exit(1)

    def create(self,port,baud):
        self.ser = serial.Serial(
            port=port,
            baudrate=baud,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            timeout=1
        )
        self.ser.flushInput()
        self.ser.flushOutput()

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
    p = u.search()
    for c in CLKS:
        for b in BAUDS:
            print(f"Clock: {c} Baud: {b}")
            os.system(f"./syn_pnr_deploy {c} {b}  > sweep_{c}_{b}.log 2>&1")
            old_bar = 0
            u.create(p,b)
            u.genVectors(VECTORS)
            for i in range(0,VECTORS,BURST):
                u.send(BURST)
                u.get(BURST)
                new = float(BURST+i)/float(VECTORS)
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
                    if(i == (VECTORS-BURST)):
                        e="\n"
                    else:
                        e=""
                    print(s,end=e)
            u.destory()
    print("PASS")

if "__main__" == __name__:
    main()

