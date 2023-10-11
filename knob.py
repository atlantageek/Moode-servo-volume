#!/usr/bin/python

import math
import sqlite3
import time
import serial.tools.list_ports
import os
import sys

ver="8.3.0"
sqldb="/var/local/www/db/moode-sqlite3.db"
max_level=512.0
min_level=20.0
ratio=(max_level - min_level) / 100.0

con=sqlite3.connect(sqldb)
cur=con.cursor()

def getPorts():
    ports = serial.tools.list_ports.comports()
    return ports

def findArduino():

    commPort = 'None'
    ports_found = getPorts()
    numConnection = len(ports_found)

    for i in range(0,numConnection):
        port = ports_found[i]
        strPort = str(port)

        if 'Arduino' in strPort:
            splitPort = strPort.split(' ')
            commPort = (splitPort[0])

    return commPort


def getVolume():
	res=cur.execute("select value from cfg_system where param='volknob'")
	result=res.fetchone()
	if result is None:
		return -1
	else:
		return result[0]
	
def setVolume(level):
	level = round(level)
	print('setVolume')
	cur.execute("UPDATE cfg_system SET value=? WHERE param='volknob'",(level,))
	con.commit()
	res=cur.execute("SELECT value FROM cfg_system WHERE param IN ('volknob','volmute','amixname','mpdmixer','cardnum','volume_mpd_max')")
	vol_knob,vol_mute,amix_name,mpd_mixer,card_num,volume_mpd_max = res.fetchall()
	if mpd_mixer == "hardware":
		cmd=f'amixer -M -c {card_num} sset {amix_name} {level} '
		os.system(cmd)
	else:
		cmd = f'mpc volume {level}'
		os.system(cmd)
	

def updateServo(volume):
	device.write((str(round(volume)) + '\n').encode('utf8'))
	print("Setting Voume:" + str(volume))
	time.sleep(1)

connectPort = findArduino() #Find Port Arduino is on.
if connectPort == 'None':
    print('Connection Issue!')
    exit(1)
print(connectPort)
device = serial.Serial(connectPort,baudrate = 9600, timeout=1)
if not(device.isOpen()):
    print("Failed to connect to port.")
    exit(1)
prev_level=None #device.readline().strip().decode("utf-8")
prev_volume=None #getVolume()
curr_volume=None
while(True):#Continuously looping
	try:
		#time.sleep(1)
		prev_volume = curr_volume
		curr_volume = int(getVolume())
		if (prev_volume != None and abs(curr_volume - prev_volume)>1):
			updateServo(curr_volume)
			continue
		read_data=device.readline().decode("utf-8").rstrip()
		input_volume=round(int(read_data))
		print("Reading Volume:" + str(input_volume) + "," + str(curr_volume))
		if (abs(input_volume - curr_volume)>1):
			setVolume(input_volume)
			curr_volume = int(getVolume())
			prev_volume=curr_volume

			continue

		print(f"prev lvl:{prev_level}, prev vol:{prev_volume}, lvl {input_volume}, vol{curr_volume}")

	except ValueError:
		continue
	except :
		print("Unexpected error:", sys.exc_info()[0])
		raise
        
          
setVolume(60)
print(getVolume())





