# Task 1

In this task, I have made a simple circuit on a bread board. The circuit itself is a linear connection of a power source (CR2032 Lithium Coin Battery), 330 ohm resistor and an LED.

<img src="/resource/task1pwd.jpeg" alt="task1 circuit" width=45%>



# Task 2

The second task did not require major circuit changes but to make a raspberry pi an energy source for the LED.  The code below flashes an LED for 2 seconds and then goes off.

Task 2 code & circuit:

![task2circuit](/resource/task2ckt.png)

    import RPi.GPIO as gp
    import time
    gp.setmode(gp.BCM)
    gp.setwarnings(False)
    gp.setup(17,gp.OUT)
    gp.output(17,gp.HIGH)
    time.sleep(2)
    gp.output(17,gp.LOW)

![task2](/resource/task2.gif)

Circuit in action



# Task 3

For this task, I was asked to add two more LEDs and make them blink in a repeatable pattern.

Task 3 code & circuit:

<img src="/resource/task3ckt.jpeg" alt="task3 circuit" width=45%>

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

![task3](/resource/task3.gif)

Circuit in action



# Task 4

Forth task asks to implement two buttons in the existing circuit. One of them must be a pause button and the other must change the sequence of LEDs’ blinks.

Task 4 code & circuit:

<img src="/resource/task4rp.jpeg" alt="task4 raspberry pi's GPIOS" width=45%> 
<img src="/resource/task4bb.jpeg" alt="task4 breadboard" width=45%>

    import RPi.GPIO as gp
    import time
    gp.setmode(gp.BCM)
    gp.setwarnings(False)

    gp.setup(17,gp.IN)
    gp.setup(27,gp.IN)

    gp.setup(26,gp.OUT)
    gp.setup(19,gp.OUT)
    gp.setup(13,gp.OUT)

    def B1():
        return gp.input(27)
    def B2():
        return gp.input(17)

    def LOW(x):
        gp.output(x,gp.LOW)
    def HIGH(x):
        gp.output(x,gp.HIGH)

    if __name__=="__main__":
        LOW(26)
        LOW(19)
        LOW(13)
        LED=[26,19,13]
        i=0
        flag=True
        while(True):
            if(B1()==0):
                while(True):
                    HIGH(LED[i])
                    time.sleep(0.2)
                    if(B1()==0):
                        while(B1()==0):
                            time.sleep(0.1)
                        break
                    if(B2()==0):
                        while(B2()==0):
                            time.sleep(0.1)
                        if(flag):
                            flag=False
                        else:
                            flag=True				
                    
                    LOW(LED[i])

                    if(flag):
                        i=i+1
                        if(i==3):
                            i=0
                    else:
                        i=i-1
                        if(i==-1):
                            i=2
                            
            
            if(B2()==0):
                while(B2()==0):
                    time.sleep(0.1)
                if(flag):
                    flag=False
                else:
                    flag=True

# Task 5

For this task, I was asked to create 2 Python modules. One of them contains Ohm’s laws to calculate current, voltage, and resistance. The second one uses these laws to check if a known value is correct.

Task 5 main code:

    import pint
    import laws
    import assert_tests as test
    from pint import UnitRegistry

    ureg=UnitRegistry()

    R=5.0 * ureg.ohm
    I=2.5 * ureg.milliampere
    V=(R*I).to('volt')

    print(V.to('volt'))
    print(V.to('millivolt'))

    test.test_voltage(3,200,0.6)

Task 5 laws module code:

    def checkR(i,v):
        return(v/i/1000)

    def checkV(i,r):
        return(r*i/1000)

    def checkI(v,r):
        return(v/r/1000)
        
    def checkSR(rs):
        res = 0
        for x in rs:
            res=res+x
        return res

Task 5 assert tests module code:

    import laws

    def test_voltage(i,r,exp):
        assert laws.checkV(i,r)==exp, "Should be:" + exp

    def test_current(v,r,exp):
        assert laws.checkI(v,r)==exp, "Should be:" + exp

    def test_resistance(i,v,exp):
        assert laws.checkR(i,v)==exp, "Should be:" + exp
