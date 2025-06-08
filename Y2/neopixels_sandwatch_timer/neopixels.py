import machine
from machine import Pin
from utime import sleep
import neopixel

startBut= machine.Pin(0,machine.Pin.IN, machine.Pin.PULL_UP)

LilBut= machine.Pin(10,machine.Pin.IN, machine.Pin.PULL_UP)
MedBut= machine.Pin(13,machine.Pin.IN, machine.Pin.PULL_UP)

DecBut= machine.Pin(15,machine.Pin.IN, machine.Pin.PULL_UP)

pin = 28
leng = 10
total = 10

ring = neopixel.NeoPixel(Pin(pin),leng)

leng-=1

clBlack = (0, 0, 0)
clGreen = (0,10,0)
clRed = (10,0,0)

def changeTimer():
    global total
    if not DecBut.value():
        if not LilBut.value():
           
            total-=1
            if total <1:
                total=1
            print(total)
            while not LilBut.value():
                sleep(0.2)
        if not MedBut.value():
            
            total-=10
            if total <1:
                total=1
            print(total)
            while not MedBut.value():
                sleep(0.2)
    else:
        if not LilBut.value():
            
            total+=1
            print(total)
            while not LilBut.value():
                sleep(0.2)
        if not MedBut.value():
            
            total+=10
            print(total)
            while not MedBut.value():
                sleep(0.2)




while (True):
    ring.fill(clBlack)
    ring.write()


    while startBut.value():
        changeTimer()

    print("starting timer for {total} seconds".format(total = total)) 
    sleep(0.5)

    fallSpeed= total/100

    for  i in range(leng+1):
        ring[leng] = clGreen
        ring.write()
        counter=0
        timeCounter=0
        while ring[i]==clBlack:
            if not startBut.value():
                break
            ring[leng-counter]=clBlack
            ring[leng-counter-1]=clGreen
            ring.write()
            sleep(fallSpeed)
            timeCounter+=fallSpeed
            counter+=1
        if not startBut.value():
            break
        if not timeCounter:
            timeCounter= total/leng -0.2   
        sleep(total/leng - timeCounter)
    if not startBut.value():
        print("timer Aborted")
        ring.fill(clBlack)
        ring.write()
        break

    print("Timer finished")

    ring.fill(clBlack)
    ring.write()
    sleep(0.3)

    for i in range(4):
        ring.fill(clRed)
        ring.write()
        sleep(0.3)

        ring.fill(clBlack)
        ring.write()
        sleep(0.3)
