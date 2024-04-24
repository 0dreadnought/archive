from inf2post import infix2Postfix,RPNcalc
import keypad_lib
import time
import oled

#seting up keypad and oled
oled_addr = 0x3C
oled_size = 1
b_flip = 1
b_invert = 1

rows=[12,16,20,21]
cols=[6,13,19,26]

chars = {
    rows[0] : {cols[0]:"1",cols[1]:"2",cols[2]:"3",cols[3]:"A"},
    rows[1] : {cols[0]:"4",cols[1]:"5",cols[2]:"6",cols[3]:"B"},
    rows[2] : {cols[0]:"7",cols[1]:"8",cols[2]:"9",cols[3]:"C"},
    rows[3] : {cols[0]:"*",cols[1]:"0",cols[2]:".",cols[3]:"D"},               
        } 
pad = keypad_lib.Keypad(cols,rows)

if (oled.oledInit(2,3,1, oled_addr, oled_size, b_flip, b_invert) == 0):
    repFlag = True
    while repFlag:
        inp = []
        flag = True
        inverter = False
        msg = "Invert: " + str(inverter)
        oled.oledWriteString(1,0,msg,2)
        #main loop goes through every button to check if pressed
        while flag:
            for x in rows:
                for y in cols:
                    z = pad.get_state(y, x)
                    if not z:
                        char = chars[x][y]
                        #effect of each button is defined here
                        #match is not available on the pi :-(
                        if char == "*":
                            inverter = not inverter
                        elif char == "A":
                            if inverter:
                                inp.append("-")
                            else:
                                inp.append("+")
                        elif char == "B":
                            if inverter:
                                inp.append("/")
                            else:
                                inp.append("*")
                        elif char == "C":
                            if inverter:
                                count1 = 0
                                count2 = 0
                                for c in inp:
                                    if c==")":
                                        count1+=1
                                    if c=="(":
                                        count2+=1
                                if count2>count1:
                                    inp.append(")")
                            else:
                                if not len(inp):
                                    inp.append("(")
                                elif inp[len(inp)-1]!=")":
                                    inp.append("(")
                        elif char == "D":
                            flag = False
                        elif char == ".":
                            if len(inp):
                                inp.pop()
                        else:
                            inp.append(char)
                        #current state output
                        oled.oledFill(0)
                        msg = "Invert: " + str(inverter)
                        oled.oledWriteString(1,0,msg,2)
                        msg = ''.join(inp)
                        oled.oledWriteString(1,3,msg,2)
                        time.sleep(0.6)
        oled.oledFill(0)

        #solution calculation and output
        inp = ''.join(inp)
        oled.oledWriteString(1,0,str(RPNcalc(infix2Postfix(inp))),0)
        oled.oledWriteString(1,2,"Again?",0)
        oled.oledWriteString(1,3,"A=N,D=Y",0)
        #loops to check 2 buttons if user wants to leave or not
        flag = True
        while flag:
            z = pad.get_state(cols[3], rows[3])
            if not z:
                flag = False
            z = pad.get_state(cols[3], rows[0])
            if not z:
                flag = False
                repFlag = False
        oled.oledFill(0)
        time.sleep(0.6)
    oled.oledFill(0)