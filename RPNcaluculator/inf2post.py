#stack datatructure implementation
class Stack:
    def __init__(self):
        self.items = []
    
    def isEmpty(self):
        if(len(self.items) == 0):
            return True
        return False

    def push(self, item):
        return self.items.append(item)

    def pop(self):
        return self.items.pop()
 
    def peek(self):
        return self.items[len(self.items)-1]
    
    def size(self):
        return len(self.items)

#few functions for internal use
def isOperator(input):
    return input in ["+","-","*","/"]

def isOperand(input):
    return input in ["0","1","2","3","4","5","6","7","8","9"]

def getPrecedence(input):
    if (input=='+' or input=='-'):
        return 1
    if (input =='*' or input=='/'):
        return 2
    return 0



def infix2Postfix(infix):
    input = list(infix)
    output = []
    stack = Stack()
    i = 0
    #iteration through the infix expression string
    while i < len(input):
        #parentesis handling
        #if inside parentesis, pushed to output as is
        if input[i] == "(":
            stack.push(input[i])
        elif input[i] == ")":
            while stack.peek() != "(":
                output.append(stack.pop())
            stack.pop() 
        #if opertor is of higher precedence then pushed to stack
        #else stack is poped to output until is
        elif isOperator(input[i]):
            if not stack.isEmpty():
                pred = getPrecedence(input[i])
                while not stack.isEmpty():
                    if getPrecedence(stack.peek()) >= pred:
                        output.append(stack.pop())
                    else:
                        break
            stack.push(input[i])
        #if operand pushed to output
        elif isOperand(input[i]):
            #logic to handle multi-digit operands
            number = ''
            while i < len(input) and isOperand(input[i]):
                number += input[i]
                i += 1
            output.append(number + ' ')
            i-=1

        i += 1

    while not stack.isEmpty():
        if stack.peek()=="(":
            stack.pop()
        else:
            output.append(stack.pop())
    
    return ''.join(output)

def RPNcalc(postfix):
    line = Stack()
    buf =[]

    #if number -> stack, if operator -> operate and stack
    for x in postfix:
        if isOperator(x):
            if line.isEmpty():
                line.push("0")
            else:
                a = float(line.pop())
                if not line.isEmpty:
                    line.push(str(a))
                else:
                    try:
                        b = float(line.pop())
                    except:
                        b = None
                    if x == "+":
                        if b == None:
                            b = 0
                        line.push(str(a+b))
                    elif x == "-":
                        if b == None:
                            b = 0
                        line.push(str(b-a))
                    elif x == "*":
                        if b == None:
                            b = 1
                        line.push(str(a*b))
                    else:
                        if b == None:
                            b = 1
                        line.push(str(b/a))
        #logic to handle multi-digit operands
        elif x!=' ':
            buf.append(x)
        else:
            line.push("".join(buf))
            buf=[]
    if line.isEmpty():
        line.push("0")
    return int(float(line.peek()))