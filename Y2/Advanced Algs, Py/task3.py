import multiprocessing

file = open("Dataset/Task 1_3/task1_3_text.txt",'r', encoding='utf-8')
text = file.read()
file.close()

def name_counter(names):
    for x in names:
        x = x.rstrip('\n')
        print({x:text.count(x)})

if __name__ == '__main__':
    proc_num = multiprocessing.cpu_count()
    names_file = open("Dataset/Task 1_3/task1_3_names.txt",'r')
    names = [x for x in names_file]
    leng = len(names)
    dstr_names = [names[i:i + leng//proc_num] for i in range(0, leng, leng//proc_num)]
    while len(dstr_names)!=proc_num:
        i=0
        dstr_names[i]+=dstr_names[len(dstr_names)-1]
        i+=1
        if i == proc_num:
            i=0
        dstr_names.pop(len(dstr_names)-1)
    processes = [multiprocessing.Process(target=name_counter, args=(dstr_names[x],)) for x in range(proc_num)]
    for p in processes:
        p.start()
    for p in processes:
        p.join()
    names_file.close()