file = open("Dataset/Task 1_4/task1_4_railway_network.csv",'r', encoding='utf-8')
data = []
while True:
    line = file.readline()
    if not line:
        break
    else:
        line = line.split(',')
        data.append(line)
MAP = {}
for y in data:
    dep = y[0].lower()
    des = y[1].lower()
    cst = int(y[2])
    if dep not in MAP:
        MAP[dep]={}
    MAP[dep][des]=cst
    if des not in MAP:
        MAP[des]={}
    MAP[des][dep]=cst
file.close()

while True:
    STRrepFlag = True
    FINrepFlag = True
    while STRrepFlag:
        start = input("Enter the departure station : ").lower()
        if start not in MAP:
            print("Station does not exist, please try again")
        elif input("Are sure your departure station is {start}? (Y/N) ".format(start=start))=="Y":
            STRrepFlag = False
    while FINrepFlag:
        finish = input("Enter the destination stantion: ").lower()
        if finish not in MAP:
            print("Station does not exist, please try again")
        elif input("Are sure your destination station is {finish}? (Y/N) ".format(finish=finish))=="Y":
            FINrepFlag = False
    to_visit = [start]
    paths_MAP = {}
    cost_MAP = {city: float('inf') for city in MAP}
    cost_MAP[start] = 0

    while to_visit:
        current_city = min(to_visit, key=cost_MAP.get)
        to_visit.remove(current_city)

        if current_city == finish:
            path =[current_city]
            while current_city in paths_MAP:
                current_city = paths_MAP[current_city]
                path.append(current_city)
            path.reverse()
            break

        for city, trvl_cost in MAP[current_city].items():
            cost = cost_MAP[current_city] + trvl_cost
            if city not in to_visit and cost < cost_MAP[city]:
                to_visit.append(city)
                paths_MAP[city] = current_city
                cost_MAP[city] = cost

    print()
    print(" --> ".join(path), end="\n\n")
    print("Total cost:",cost_MAP[finish])
    if input("Do you wish to quit? (Y/N) ")=="Y":
        break