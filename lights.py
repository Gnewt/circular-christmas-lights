#!/usr/bin/python

"""
Title: Christmas Lights Server
Date: 06/25/12
Author: Gnewt <gnewt@gnewt.at>
License: WTFPL (http://sam.zoy.org/wtfpl/)
"""

import serial
import SocketServer
from random import choice
from sys import argv, exit

# Lights start out black (off)-- you can change this!
lights = [(0, 0, 0)]*36

# Pure random colors are kind of ugly.

#acceptable_colors = [(0xF, 0x0, 0x0), (0x0, 0x0, 0x0), (0xF, 0x0, 0x0), (0x0, 0xF, 0x0),
#                    (0x0, 0x0, 0xF), (0x0, 0xF, 0xF), (0xF, 0x0, 0xF), (0xF, 0xF, 0x0),
#                    (0xA, 0x3, 0xD), (0xF, 0x1, 0x0), (0xF, 0x7, 0x2)]

acceptable_colors = {'white': (0xF, 0xF, 0xF), 
                     'black': (0x0, 0x0, 0x0), 'red': (0xF, 0x0, 0x0),
                     'green': (0x0, 0xF, 0x0), 'blue': (0x0, 0x0, 0xF),
                     'cyan': (0, 0xF, 0xF), 'magenta': (0xF, 0x0, 0xF),
                     'yellow': (0xF, 0xF, 0x0),
                     'purple': (0xA, 0x3, 0xD),
                     'orange': (0xF, 0x1, 0x0),
                     'warm white': (0xF, 0x7, 0x2)}
                    




class HandleConnections(SocketServer.BaseRequestHandler):
    def handle(self):
        color_choice = choice(acceptable_colors.keys())
        color = acceptable_colors[color_choice]
        self.request.sendall("Your color is: %s\n" % color_choice)
        lights.remove(lights[0])
        lights.append(color)
        arduino.write(chr(color[0]))
        arduino.write(chr((color[1] << 4) + color[2]))
        print_lights(lights)

def print_lights(lights):
    for i in range(len(lights)):
        print(lights[i]),
        if (i + 1) % 6 == 0:
            print
    print

if __name__ == "__main__":
    try:
        arduino = serial.Serial(argv[1], baudrate=9600, timeout=2)
    except serial.SerialException:
        print("Serial port %s could not be opened.", argv[1])
        exit(-1)
    except:
        print("Usage: lights.py serialport")
        exit(-1)
        
    # Change these to whatever you want
    HOST, PORT = "0.0.0.0", 11736
    server = SocketServer.TCPServer((HOST, PORT), HandleConnections)
    server.serve_forever()
