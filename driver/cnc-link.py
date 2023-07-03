#-------------------------------------------------
# This script is used to feed G-Code files to
# the Gerber Prototyping CNC.
#
# Designed to run on Windows
#
# args: <filename>
#
# Dependencies: Serial
#
# Author: Andrew Gerber
#-------------------------------------------------

import sys
#import glob
import re
from time import sleep
from typing import List
import serial
import serial.tools.list_ports


GCODE_ACK_RE  = "^ACK$"
GCODE_NACK_RE = "^NACK.*$"


def listPorts() -> List[dict]:
    ports = []
    for port in serial.tools.list_ports.comports():
        port_id = re.search(r'\d+',port[0]).group()
        ports.append({'id': port_id, 'name': port[0], 'desc': port[1]})
    return sorted(ports, key=lambda item: item['id'])
    #if sys.platform.startswith('win'):
    #    ports = ['COM%s' % (i + 1) for i in range(256)]
    #elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
    #    # this excludes your current terminal "/dev/tty"
    #    ports = glob.glob('/dev/tty[A-Za-z]*')
    #elif sys.platform.startswith('darwin'):
    #    ports = glob.glob('/dev/tty.*')
    #else:
    #    raise EnvironmentError('Unsupported platform')
    #result = []
    #for port in ports:
    #    try:
    #        #s = serial.Serial(port)
    #        #s.close()
    #        result.append(port)
    #    except (OSError, serial.SerialException):
    #        pass
    #result.sort()
    #return result


def initSerial(baudrate=9600) -> serial.Serial:
    ports = listPorts()
    if not ports:
        print("No serial ports detected")
        exit(0)
    else:
        print(f"Found {len(ports)} COM port(s):\n")
        port = ''
        while not port:
            for p in ports:
                print(f"  ({p['id']})  {p['name']} - {p['desc']}")
            print()
            x = input('Select a port: ')
            for p in ports:
                if x == p['id']:
                    port = p['name']
                    break
            if not port:
                print("Invalid selection. Must be integer from list above")
            print()
    com = serial.Serial(port, baudrate=baudrate)
    print(f"Connected to {port}")
    print()
    return com


class Response:
    def __init__(self, ack:bool, msg:str):
        self.ack = ack
        self.msg = msg


def sendMsg(msg:str, com:serial.Serial, echo:bool=True, lineNum="") -> Response:
    msg = msg.strip()
    if not msg:
        return None
    com.write((msg+"\r").encode('ascii'))
    if echo:
        #print(msg,end="\r")
        print(f"DEBUG Line {lineNum}: '{msg}'")
    reply = ''
    while (not reply) or (reply == msg):
        #reply = str(com.read_until(), 'ascii').strip()
        print('DEBUG reply: \'', end='')
        reply = ''
        char = ''
        while char != '\r' and char != '\n':
            #print(char,end='')
            reply += char
            while com.inWaiting() < 1:
                pass
            char = str(com.read(), 'ascii')
        print('\'')
        com.flushInput()
    if echo:
        print(f"{lineNum} {reply}: {msg}")
    ack = re.search(GCODE_ACK_RE, reply)
    if not ack:
        raise ValueError()
    return Response(ack, reply)


if __name__ == "__main__":
    if 2 != len(sys.argv):
        print("Command takes 1 argument specifying the G-Code file")
        sys.exit(1)
    try:
        with open(sys.argv[1], "r") as file:
            com = initSerial()
            sleep(0.5)
            com.reset_input_buffer()
            #try:
            #    sendMsg("M111 D0", com)
            #except ValueError:
            #    pass
            com.reset_input_buffer()
            line = file.readline()
            lineNum = 1
            while line:
                sendMsg(line, com, lineNum=lineNum)
                line = file.readline()
                lineNum += 1
            print("Reached end of file")
            #sendMsg("M111 D1", com)
    except FileNotFoundError as ex:
        print(f"Error loading file '{sys.argv[1]}'")
        exit(1)
    except ValueError as ex:
        print("Abort")
        exit(1)
