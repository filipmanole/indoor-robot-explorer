#!/usr/bin/python

# MIT License
# 
# Copyright (c) 2017 John Bryan Moore
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import time
import VL53L0X
import RPi.GPIO as GPIO

# GPIO for Sensor 1 shutdown pin
sensor1_shutdown = 26
# GPIO for Sensor 2 shutdown pin
sensor2_shutdown = 20

sensor3_shutdown = 13

GPIO.setwarnings(False)

# Setup GPIO for shutdown pins on each VL53L0X
GPIO.setmode(GPIO.BCM)
GPIO.setup(sensor1_shutdown, GPIO.OUT)
GPIO.setup(sensor2_shutdown, GPIO.OUT)
GPIO.setup(sensor3_shutdown, GPIO.OUT)

# Set all shutdown pins low to turn off each VL53L0X
GPIO.output(sensor1_shutdown, GPIO.LOW)
GPIO.output(sensor2_shutdown, GPIO.LOW)
GPIO.output(sensor3_shutdown, GPIO.LOW)

time.sleep(0.50)

# Set address 0x2b for ToF1 
GPIO.output(sensor1_shutdown, GPIO.HIGH)
tof1 = VL53L0X.VL53L0X(i2c_address=0x29)
tof1.change_address(0x2b)
tof1.open()

time.sleep(0.50)

# Set address 0x2a for ToF2
GPIO.output(sensor2_shutdown, GPIO.HIGH)
tof2 = VL53L0X.VL53L0X(i2c_address=0x29)
tof2.change_address(0x2a)
tof2.open()

time.sleep(0.50)

# Let default address for ToF3 0x29
GPIO.output(sensor3_shutdown, GPIO.HIGH)
tof3 = VL53L0X.VL53L0X(i2c_address=0x29)
tof3.open()

tof1.start_ranging(VL53L0X.Vl53l0xAccuracyMode.BETTER)
tof2.start_ranging(VL53L0X.Vl53l0xAccuracyMode.BETTER)
tof3.start_ranging(VL53L0X.Vl53l0xAccuracyMode.BETTER)


timing = tof1.get_timing()
if timing < 20000:
    timing = 20000
print("Timing %d ms" % (timing/1000))

for count in range(1,101):
    distance = tof1.get_distance()
    if distance > 0:
        print("sensor %d - %d mm, %d cm, iteration %d" % (1, distance, (distance/10), count))
    else:
        print("%d - Error" % 1)

    distance = tof2.get_distance()
    if distance > 0:
        print("sensor %d - %d mm, %d cm, iteration %d" % (2, distance, (distance/10), count))
    else:
        print("%d - Error" % 2)

    distance = tof3.get_distance()
    if distance > 0:
        print("sensor %d - %d mm, %d cm, iteration %d" % (3, distance, (distance/10), count))
    else:
        print("%d - Error" % 2)

    time.sleep(timing/1000000.00)

tof1.stop_ranging()
GPIO.output(sensor1_shutdown, GPIO.LOW)
tof2.stop_ranging()
GPIO.output(sensor2_shutdown, GPIO.LOW)
tof3.stop_ranging()
GPIO.output(sensor3_shutdown, GPIO.LOW)

tof1.close()
tof2.close()
tof3.close()

