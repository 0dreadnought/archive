import RPi.GPIO as gp
import time
gp.setmode(gp.BCM)
gp.setwarnings(False)

gp.setup(27,gp.OUT)
gp.setup(17,gp.OUT)
gp.setup(15,gp.OUT)

gp.output(27,gp.LOW)
gp.output(17,gp.LOW)
gp.output(15,gp.LOW)

while(True):
	time.sleep(0.25)
	gp.output(17,gp.HIGH)
	time.sleep(1)
	gp.output(17,gp.LOW)
	gp.output(15,gp.HIGH)
	time.sleep(1)
	gp.output(15,gp.LOW)
	gp.output(27,gp.HIGH)
	time.sleep(1)
	gp.output(27,gp.LOW)
	time.sleep(1)