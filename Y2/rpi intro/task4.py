import RPi.GPIO as gp
import time
gp.setmode(gp.BCM)
gp.setwarnings(False)

gp.setup(17,gp.IN)
gp.setup(27,gp.IN)

gp.setup(26,gp.OUT)
gp.setup(19,gp.OUT)
gp.setup(13,gp.OUT)

gp.output(26,gp.LOW)
gp.output(19,gp.LOW)
gp.output(13,gp.LOW)


def B1():
	global statewait
	
	if (gp.input(27)==0):
		while(gp.input(27)==0):
			time.sleep(0.1)
		if(statewait==0):
			statewait+=1
		else:
			statewait-=1
	
def B2():
	global statedir
	
	if (gp.input(17)==0):
		while(gp.input(17)==0):
			time.sleep(0.1)
		if(statedir==0):
			statedir+=1
		else:
			statedir-=1	
			
def wait():
	global i
	global statewait
	
	while(1):
		checkstate()
		if(statewait):
			time.sleep(0.1)
		else:
			gp.output(13,gp.LOW)
			gp.output(26,gp.LOW)
			gp.output(19,gp.LOW)
			break
        
	
def checkstate():
	B1()
	B2()	

def run():
	global i
	global statedir
	global statewait	

	if(statedir):
		i=i+1
		if(i==3):
			i=0
	else:
		i=i-1	
		if(i==-1):
			i=2	

	gp.output(LED[i],gp.HIGH)
	time.sleep(0.2)
	checkstate()
	if(statewait==0):
		gp.output(LED[i],gp.LOW)

if __name__=="__main__":
    LED=[26,19,13]
    i=0
    statewait=0
    statedir=0
    while(1):
        if(gp.input(27)==0):
            while(1):
                if(statewait):
                    wait()
                run()