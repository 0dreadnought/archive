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