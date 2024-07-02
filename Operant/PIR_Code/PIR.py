#This should record all the data from the arduino to file and print to the python console

#pip install pyserial before running

import serial
import datetime
import os

#Update port to match port of the arduino
arduino = serial.Serial(port='/dev/ttyUSB1', baudrate=57600, timeout=.1)

#Output directory - need to run as admin if using C:/
output_d = "/home/operant/Circadian/PIR_Output_Alcohol/"

try:
  os.mkdir(output_d)
except:
  print("already exists or need admin rights")

os.chdir(output_d)

now = datetime.datetime.now()
output_filename = "PIR_Output_Alcohol" + now.strftime("%Y-%m-%d_%H-%M-%S")+".txt"
Counter = 0

while True:
  value = arduino.readline()
  value = value.decode()
  if value != '':
    Counter=Counter+1
    now = datetime.datetime.now()
    output_f = open(output_filename,"a")
    a = output_f.write(now.strftime("%Y-%m-%d %H:%M:%S")+","+value)
    print(value)
    output_f.close()
    if Counter>86400:
      output_filename = "PIR_Output_Alcohol" + now.strftime("%Y-%m-%d_%H-%M-%S")+".txt"
      Counter = 0
