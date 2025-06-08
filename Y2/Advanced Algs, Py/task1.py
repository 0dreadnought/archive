from itertools import product
caps = "ABCDE"
lows = "abcde"
digs = "12345"
syms = "$&%"
alls = caps+lows+digs+syms
sols = []
combos = product(alls,repeat=int(input("Please enter length of the password: ")))

for combo in combos:
    if combo[0].isalpha():
        cs=0
        ls=0
        ds=0
        ss=0
        for x in combo:
            if x in caps:
                cs+=1
            elif x in lows:
                ls+=1
            elif x in digs:
                ds+=1
            else:
                ss+=1
        if 0<cs<3 and 0<ls and 0<ds and 0<ss<3:
            sols.append(combo)

print(len(sols))