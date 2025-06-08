# DD_assignment

## Task

The task was to write an RPN calculator, that would take an infix expression, turn it to RPN and calculate it.
The input must be entered using keypad, interface for each must also be part of a sollution
The output must be displayed on the OLED, interface for each must be wrapped into a python module.

## Implementation

### main.py

main loop iterates through every button on the key pad to check if it is pressed
and if so, depending on the button the input expression will be extended, shortened, 
calculated or some buttons will change in functionality.
Numbers always symbolize numbers. * is used to change inverter flag's state. # erases last element of input.
A is either + if inverter is False or - if inverter is True. B is either * or /. C is either ( or ).

```python
    while flag:
        for x in rows:
            for y in cols:
                z = pad.get_state(y, x)
                if not z:
                    char = chars[x][y]
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
                            inp.append(")")
                        else:
                            inp.append("(")
                    elif char == "D":
                        flag = False
                    elif char == ".":
                        if len(inp):
                            inp.pop()
                    else:
                        inp.append(char)
                    oled.oledFill(0)
                    msg = "Invert: " + str(inverter)
                    oled.oledWriteString(1,0,msg,2)
                    msg = ''.join(inp)
                    oled.oledWriteString(1,3,msg,2)
                    time.sleep(0.6)
                    
```

Loop to check 2 buttons. Made for user to be able to use calculator again or exit the program:

```python
    while flag:
        z = pad.get_state(rows[3], cols[3])
        if not z:
            flag = False
        z = pad.get_state(rows[0], cols[3])
        if not z:
            flag = False
            repFlag = False
```

### INFIX to RPN implementation and RPN calculator

infix to rpn is made using stack, iterating though the given string and either pushing element to the output,
or pushing it to the stack to later put operators in the right order:

```python
    def infix2Postfix(infix):
        input = list(infix)
        output = []
        stack = Stack()
        i = 0
        while i < len(input):
            if input[i] == "(":
                stack.push(input[i])
            elif input[i] == ")":
                while stack.peek() != "(":
                    output.append(stack.pop())
                stack.pop() 
            elif isOperator(input[i]):
                if not stack.isEmpty():
                    pred = getPrecedence(input[i])
                    while not stack.isEmpty():
                        if getPrecedence(stack.peek()) >= pred:
                            output.append(stack.pop())
                        else:
                            break
                stack.push(input[i])
            elif isOperand(input[i]):
                number = ''
                while i < len(input) and isOperand(input[i]):
                    number += input[i]
                    i += 1
                output.append(number + ' ')
                i-=1

            i += 1

        while not stack.isEmpty():
            output.append(stack.pop())
        
        return ''.join(output)
```

Calculator itself is also implemented using stack, where  operands are pushed on to the stack,
and when operator is encountered operands are taken from the stak, operation is performed,
and the result is pushed back to stack:

```python
    def RPNcalc(postfix):
        line = Stack()
        buf =[]

        for x in postfix:
            if isOperator(x):
                a = float(line.pop())
                b = float(line.pop())
                if x == "+":
                    line.push(str(a+b))
                if x == "-":
                    line.push(str(b-a))
                if x == "*":
                    line.push(str(a*b))
                if x == "/":
                    line.push(str(b/a))
            elif x!=' ':
                buf.append(x)
            else:
                line.push("".join(buf))
                buf=[]

        return int(float(line.peek()))
```

### Keypad Lib

Only one function besides constructor and destructor.
In __init__ GPIO ports are seted up using provided port numbers 

```python
    def __init__(self,cols:list[4],rows:list[4]):

        GPIO.setmode(GPIO.BCM)

        GPIO.setup(rows[0], GPIO.IN, pull_up_down=GPIO.PUD_UP)
        GPIO.setup(rows[1], GPIO.IN, pull_up_down=GPIO.PUD_UP)
        GPIO.setup(rows[2], GPIO.IN, pull_up_down=GPIO.PUD_UP)
        GPIO.setup(rows[3], GPIO.IN, pull_up_down=GPIO.PUD_UP)

        GPIO.setup(cols[0], GPIO.OUT)
        GPIO.setup(cols[1], GPIO.OUT)
        GPIO.setup(cols[2], GPIO.OUT)
        GPIO.setup(cols[3], GPIO.OUT)      

        print("init done")
```

and the get_state() function checks if the button of the given row and collumn is pressed:

```python
    def get_state(self,col,row):
        GPIO.output(col, GPIO.LOW)
        x = GPIO.input(row)
        GPIO.output(col, GPIO.HIGH)
        return x
```

### OLED lib

OLED module is provided in C and everything required to do was to wrap it into a python module,
which included turning every public function into a python object and parsing arguments the right way:

```c
    static PyObject * _oledInit(PyObject * self, PyObject * args)
    {
    int iChannel, iAddr, iType, bFlip, bInvert;
    int sdaL, sclL;

    PyArg_ParseTuple(args,"iiiiibb",&sdaL,&sclL,&iChannel,&iAddr,&iType,&bFlip,&bInvert);
    ...
    return Py_BuildValue("i",0);
    }
```

The only change made to the module is that now ports are passed as arguments in init.
Also include few other functions used to build the module:

```c
    static struct PyMethodDef OMeths [] = {
    {"oledInit", (PyCFunction)_oledInit, METH_VARARGS, "oledINITfunction"},
    {"oledShutdown",(PyCFunction)_oledShutdown,METH_NOARGS,"oledShutdown"},
    {"oledWriteString",(PyCFunction)_oledWriteString,METH_VARARGS,"oledWriteString"},
    {"oledFill",(PyCFunction)_oledFill,METH_VARARGS,"oledFill"},
    {"oledDrawLine",(PyCFunction)_oledDrawLine,METH_VARARGS,"oledDrawLine"},
    {"oledDrawCircle",(PyCFunction)_oledDrawCircle,METH_VARARGS,"oledDrawCircle"},
    
        {NULL,NULL,0,NULL}
    };

    static struct PyModuleDef oledM = {
        PyModuleDef_HEAD_INIT,
        "oled",
        "OM",
        -1,
        OMeths
    };

    PyMODINIT_FUNC PyInit_oled(void){
        PyObject *module = PyModule_Create(&oledM);

        return module;
    }
```

## Result

![](resource/calculator.mp4)

view from terminal:

![screen](/resource/scr1.png)

my connections ro the raspberry pi, numbers are GPIO ports

![scheme](/resource/scheme.png)
