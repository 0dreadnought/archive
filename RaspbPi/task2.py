import RPi.GPIO as gp
import time
gp.setmode(gp.BCM) #Here and in every other task I used BCM mode to refer to pins
gp.setwarnings(False)

gp.setup(17,gp.OUT) #initialization of a pin

gp.output(17,gp.HIGH) #passing voltage through pun, lighting up the LED
time.sleep(2)
gp.output(17,gp.LOW) #stoping the current
