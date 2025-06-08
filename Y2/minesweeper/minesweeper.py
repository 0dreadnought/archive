"""
  File:    minesweeper.py
  
  Author:  Utenov Ilyas (ilyas2.utenov@live.uwe.ac.uk)
  Date:     24/12/2023
  Partner:  I worked alone
  Course:   UFCFCS-30-2 - Digital Design 23sep_1
  
  Summary of File:
  
  This code is a Python console-based video game.

  I confirm that this assignment is my own work. Where I have referred to academic sources,
  I have provided in-text citations and included the sources in the final reference list.
"""
from abc import abstractmethod
import time
import random
import os

"""
    Input class

    Description:
        Abstract class with an abstraact method read(self)
    
    Parameters: 
        None
"""

class Input:
    @abstractmethod
    def read(self):
        pass

"""
    handleInput(Input) class

    Description:
        Class that inherits from the Input class,
        and provides functionality to process user input in form of read(self) method

    Parameters:
        Input class

        

    read(self) method

    Description:
        Method takes user input and passes it

    Parameters:
        self
    
    Returns:
        dictionary
"""

class handle_Input(Input):
    def read(self):
        command=row=col=None
        nums=["{x}".format(x=x) for x in range(bsize)]
        while command not in ["O","o","f","F"]:
            command=input("Select operation('O' to Open tile, 'F' to put or take off a Flag):")
        while (row not in nums):    
            row=input("Select column: ")
        while (col not in nums):
            col= input("Select row: ")
        return{
            'row':int(row),
            'col':int(col),
            'command':command
        }

"""
    diff_Select function

    Description:
        Function takes user input and returns board size and total number of mines
    
    Parameters:
        None
    
    Returns:
        interger and integer
"""

def diff_Select():
    print("Difficulties: 1.Beginner(8x8:10) 2.Intermediate(15x15:40) 3.Advanced(22x22:99)")
    diff=input("Select desired difficulty: ")
    if(diff in ["Beginner","1"]):
        return(8,11)
    elif(diff in ["Intermediate","2"]):
        return(15,41)
    elif(diff in ["Advanced","3"]):
        return(22,100)
    else:
        os.system("cls||clear")
        print("Please select a valid difficulty")
        return(diff_Select())
    
"""
    gen_Board function

    Description:
        This function randomly places mines on the board,
        as well as giving a number of mines around for each non-mine tile 

    Parameters:
        None

    Returns:
        nothing
"""

def gen_Board():
    random.seed(time.time())
    global mines
    mines=[0 for k in range(mines_num-1)]
    global board
    board = [[0 for col in range(bsize)] for row in range(bsize)]
    col=random.randrange(bsize)
    row=random.randrange(bsize)
    for k in range(1,mines_num):
        while(board[col][row] =="[M]"):
            col=random.randrange(bsize)
            row=random.randrange(bsize)
        board[col][row] = "[M]"
        mines[k-1]=col*bsize+row
    mines.sort() 
    for col in range(bsize):
        for row in range(bsize):
            tile=0
            if(board[col][row]=="[M]"):
                pass
            else:
                for x in range(-1,2):
                    for y in range(-1,2):
                        if (-1<col+x<bsize and -1<row+y<bsize):
                            if (board[col+x][row+y]=="[M]"):
                                tile +=1            
                board[col][row]="[{tile}]".format(tile=tile)

"""
    take_input

    Description:
        This function calls for handle_Input class's read(self) method,
        stores returned dictionary and then passes values from it to another function
    
    Parameters:
        input_hadler: handle_Input class
    
    Returns:
        nothing
"""

def take_Input(input_handler):
    user_input=input_handler.read()
    flip_Tile(user_input["command"],user_input["col"],user_input["row"],True)

"""
    flip_Tile(oper,col,row,flag)

    Description:
        This function, based on user input will open a selected tile or
        put or take down a flag on selected tile by adding tile to respective list.

    Parameters:
        oper: selected operation
        col: selected column
        row: selected row
        flag: used to avoid infininte loops

    Returns:
        nothing
"""

def flip_Tile(oper,col,row,flag):
    tile=col*bsize+row
    if (oper in ["O","o"]):
        if (tile in flags):
            pass
        else:
            if (tile not in opened):
                opened.append(tile)
                if (board[col][row]=="[0]"):
                    for x in range(-1,2):
                        for y in range(-1,2):
                            if (-1<col+x<bsize and -1<row+y<bsize):
                                flip_Tile(oper,col+x,row+y,False)
            elif(flag):
                FM=0
                for x in range(-1,2):
                        for y in range(-1,2):
                            if (-1<col+x<bsize and -1<row+y<bsize):
                                if(((col+x)*bsize+row+y) in flags):
                                    FM+=1
                if("[{FM}]".format(FM=FM)==board[col][row]):
                    flip_Around(col,row)
    else:
        if(tile in flags):
            flags.remove(tile)
        elif(len(flags) != mines_num-1):
            flags.append(tile)
            flags.sort()

"""
    flip_Around

    Description:
        Opens every tile around one. 

    Parameters:
        col: selected column
        row: selected row

    Returns:
        nothing
"""

def flip_Around(col,row):
    for x in range(-1,2):
        for y in range(-1,2):
            if (-1<col+x<bsize and -1<row+y<bsize and (col+x)*bsize+row+y not in flags ):
                if(board[col+x][row+y]=="[0]"):
                    flip_Tile("O",col+x,row+y,False)
                if ((col+x)*bsize+row+y not in opened):
                    opened.append((col+x)*bsize+row+y)

"""
    print_Score

    Description:
        Calculates and prints out user's score based on time it took for user to solve the puzzle.

    Parameters:
        timer: moment when current play began
    
    Returns:
        nothing
"""

def print_Score(timer):
    score = int(1000000/(time.time()-timer))
    print("\nYour Score is {score}".format(score=score))

"""
    print_Board

    Description:
        Shows current state of the minefield.

    Parameters:
        None

    Returns:
        nothing
"""

def print_Board():
    os.system("cls||clear")
    print("",end="   ")
    for i in range(bsize):
        print(i,end=(3-len(str(i)))*" ")
    print()
    for i in range(bsize):
        print(i,end=(2-len(str(i)))*" ")
        for j in range(bsize):
            if (i*bsize+j in opened):
                print(board[i][j],end="")
            elif (i*bsize+j in flags):
                print("[F]",end="")
            else: print("[ ]",end="")
        print()
    print("Mines left:{n}".format(n=mines_num-len(flags)-1))

"""
    Main code

    Description:
        Starts the game and checks if the minefield is fit to continue the game, otherwise declares the user
        victorious or defeated and asks if one wishes to play again. If so, the game restarts, otherwise the
        programm terminates.  
"""

if __name__ == "__main__":
    global opened
    global flags
    global bsize
    global mines_num
    input_handler=handle_Input()
    restart_Flag=True
    while(restart_Flag):
        os.system("cls||clear")
        bsize,mines_num = diff_Select()
        timer = time.time()
        opened=[]
        flags=[]
        gen_Board()
        print_Board()
        while (not set(opened) & set(mines) and (len(opened)!=(bsize*bsize-mines_num+1) or flags != mines)):
            take_Input(input_handler)
            print_Board()
        if(not set(opened) & set(mines)):
            print("VICTORY")
            print_Score(timer)
        else:
            print("DEFEAT")
        if(input("Do you wish to continue?(Y/N) ")in ["Y","Yes","YES","yes","y"]):
            pass
        else:
            restart_Flag=False