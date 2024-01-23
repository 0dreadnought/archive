num=int(input("Plz enter a num: "))
for i in range(2, num+1):
    if num==i:
        print("Is Prime")
    elif num%i==0:
        print("Is not Prime")
        break