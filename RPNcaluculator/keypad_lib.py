import RPi.GPIO as GPIO

class Keypad:
    def __init__(self,cols:list[4],rows:list[4]):

        GPIO.setmode(GPIO.BCM)
        #gpio setup
        GPIO.setup(rows[0], GPIO.IN, pull_up_down=GPIO.PUD_UP)
        GPIO.setup(rows[1], GPIO.IN, pull_up_down=GPIO.PUD_UP)
        GPIO.setup(rows[2], GPIO.IN, pull_up_down=GPIO.PUD_UP)
        GPIO.setup(rows[3], GPIO.IN, pull_up_down=GPIO.PUD_UP)

        GPIO.setup(cols[0], GPIO.OUT)
        GPIO.setup(cols[1], GPIO.OUT)
        GPIO.setup(cols[2], GPIO.OUT)
        GPIO.setup(cols[3], GPIO.OUT)      

        print("init done")


    def get_state(self,col,row):
        #check if button is presed
        GPIO.output(col, GPIO.LOW)
        x = GPIO.input(row)
        GPIO.output(col, GPIO.HIGH)
        return x



    def __del__(self):
        #cleanup
        GPIO.cleanup()
        print("cleanup done")