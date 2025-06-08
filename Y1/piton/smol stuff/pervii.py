number=input("Please enter positive integer:")
sum=0
flag=0
while flag==0:
    if not number.isnumeric():
        number=input("Please enter POSITIVE INTEGER:")
    elif int(number)==0:
        number=input("Please enter POSITIVE INTEGER:")
    else: 
        flag=flag+1
    
for num in number:
    sum=sum+int(num)
print(sum)
