first=int(input("Write a first number for a range plz: "))
second=int(input("Write a second number for a range plz: "))
for first in range(first, second+1):
    if first%2==1:
        print(first)