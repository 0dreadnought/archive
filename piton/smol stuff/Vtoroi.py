import random
def main():
    random.seed()
    result=0
    maxx=input("Please enter highest dice value, it must be an interger from 2 to 21:")
    rolls=input("Please enter number of rols, it must be an integer from 1 to 10:") 
    flag=0
    while flag==0:
        if not maxx.isnumeric():
             maxx=input("Please enter valid value for highest dice value, it must be an INTERGER from 2 to 21:")
        elif    not rolls.isnumeric():
            rolls=input("Please enter valid value for number of rols, it must be an INTEGER from 1 to 10:") 
        elif int(maxx)<=1 or int(maxx)>=22:
            maxx=input("Please enter valid value for highest dice value, it must be an interger FROM 2 TO 21:")
        elif int(rolls)<=0 or int(rolls)>=11:
            rolls=input("Please enter valid value for number of rols, it must be an integer FROM 1 TO 10:") 
        else: 
            flag=flag+1
    for i in range(int(rolls)):
        number=random.randrange(1,int(maxx)+1)
        print(number)
        result=result+number
    print("Final sum is:"+ str(result))

if __name__ == "__main__":
    main()
    
