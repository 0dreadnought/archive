file = open("Dataset/Task 1_2/letters.txt",'r')
letters = file.read()
file.close()

num = int(input("Number of repetead elements: "))
max_str = ""
flag = False
strs =[]

for i in range(len(letters)-1):
    j=i
    string = letters[i]
    while len(set([x for x in string if string.count(x) > 1]))<=num:
        if len(max_str)<len(string):
            max_str = string
        elif len(max_str)==len(string):
            strs.append(string)
        j+=1
        if j == len(letters):
            flag = True
            break
        string +=letters[j]
    if flag:
        break

strs.append(max_str)
max_len = len(max_str)
x=0
while x < len(strs):
    if len(strs[x])<max_len:
        strs.pop(x)
        x-=1
    x+=1
print(max_len)
print('\n'.join(strs))