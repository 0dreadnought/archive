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
