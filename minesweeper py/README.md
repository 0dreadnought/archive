# Minesweeper



## About this game

Minesweeper is a logic puzzle video game. Open up tiles and use hints from them to locate mines. Mark them with flags to successfully neutrolyze the minefield!

## Gameplay features

Open tiles by entering its column and row number. If there is no mine on the tile, opened tile will revial how many mines there are around it.

Use information from other tiles to picture positions of mines.

Mark mines with flags, by selecting Flag operation and entering desired column and row.

If you open up a tile without any mines around it will automaticaly open each tile around.

You can used Open operation on already opened tiles. If you put right ammount of flags around the tile, all other tiles will open. However if any of the flags are not covering mine spot, mine will be opened, resulting in defeat.

## Code

The code is written in python and imports abc, os, random and time libraries to create abstract methods, clearing terminal and generating random numbers respectively.

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


    def take_Input(input_handler):
        user_input=input_handler.read()
        flip_Tile(user_input["command"],user_input["col"],user_input["row"],True)

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


## Screenshots

Example of victory

![screen](/assets/1.png)

![screen](/assets/2.png)

![screen](/assets/3.png)

![screen](/assets/4.png)

time skip

![screen](/assets/5.png)

![screen](/assets/6.png)

time skip

![screen](/assets/7.png)

![screen](/assets/8.png)

![screen](/assets/9.png)

![screen](/assets/11.png)

Example of defeat

![screen](/assets/12.png)

![screen](/assets/13.png)
