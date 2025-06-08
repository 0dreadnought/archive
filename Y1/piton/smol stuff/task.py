c=-1
first=int(input("Write a first number for a range plz: "))
second=int(input("Write a second number for a range plz: "))
for first in range(first, second+1):
    if first%3==0:
        c+=1
print(c)