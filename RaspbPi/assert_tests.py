import laws

def test_voltage(i,r,exp):
    assert laws.checkV(i,r)==exp, "Should be" + exp

def test_current(v,r,exp):
    assert laws.checkI(v,r)==exp, "Should be" + exp

def test_resistance(i,v,exp):
    assert laws.checkR(i,v)==exp, "Should be" + exp
