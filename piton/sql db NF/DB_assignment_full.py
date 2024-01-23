import mysql.connector
import pandas
from datetime import datetime
mydb= mysql.connector.connect(
    host="localhost",
    user="root",
    password="",
    database="marvel"
)
mycursor= mydb.cursor()

mycursor.execute("CREATE TABLE comic_series (comic_id VARCHAR(255), comic_name VARCHAR(255), active_years VARCHAR(255))")
mydb.commit()
mycursor.execute("CREATE TABLE comic_issues (issue_id VARCHAR(255),issue_title VARCHAR(255), publish_date VARCHAR(255), issue_description VARCHAR(255), penciler VARCHAR(255), writer VARCHAR(255), cover_artist VARCHAR(255), Imprint VARCHAR(255), Format VARCHAR(255), Rating VARCHAR(255), Price VARCHAR(255))")
mydb.commit()
mycursor.execute("CREATE TABLE comics (comic_id VARCHAR(255), issue_id VARCHAR(255))")
mydb.commit()

file = pandas.read_csv("Marvel_Comics.csv")
i=1 #The comic books are pre-sorted in alphabetic order in the csv file.
    #This means that no comic will randomly occure in the midle of the table, after it was once.
    #By knowing that a simple counter can be set as an ID for all the comic series

sql="INSERT INTO comic_series (comic_id,comic_name,active_years) VALUES (%s,%s,%s)"
    #The programm will read every row of a csv file and will make a unique id for each comic series, while also writing it in the database.
for x,row in file.iterrows():
    if i==1:
        #Futere iterations will need to compare current value of a variable with previous, so we need to set a starting point.
        val=(i,row[0],row[1])
        mycursor.execute(sql,val)
        mydb.commit()
        i+=1
    if val[1] != row[0]:
        #From now on the program will check if comic name of current row is different from the previous.
        #If it is not, then the program will just ignore until a new name is faced.
        #If it is, that means that a new series of comics is coming, hence a new id required.
        val=(i,row[0],row[1])
        mycursor.execute(sql,val)
        mydb.commit()
        i+=1
print("Work done")

i=1

sql="INSERT INTO comic_issues (issue_id,issue_title,publish_date,issue_description,penciler,writer,cover_artist,Imprint,Format,Rating,Price) VALUES (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)"
for x,row in file.iterrows():
    #The program again will read each row of a file and to each unique issue title it will give an ID, while also writing it in the database.
    if i==1:
        #As before the program will need to compare values of previous data, so we need to provide a starting point for it.
        goodPen=str.split(str(row[5]),",")
        goodWri=str.split(str(row[6]),",")
        goodCov=str.split(str(row[7]),",")
        #There might be more than one penciler,writer or cover artist for a comic so we need to distinguish them and make a record for each one.
        for o in goodPen:
            for p in goodWri:
                for l in goodCov:
                    row[3] = row[3].replace('-', '') 
                    if row[3].strip() == "None":
                        date = None
                    else:
                        date = datetime.strptime(row[3], '%B %d, %Y')
                        date =  date.strftime('%Y-%m-%d')
                    #The date format in the file is different from what SQL operates with, so it needs to be converted first, for further queries.
                    val=(i,row[2],date,row[4],o,p,l,row[8],row[9],row[10],row[11])
                    mycursor.execute(sql,val)
                    mydb.commit()
                    
        i+=1    
            
    if val[1] != row[2]:
        #From now on programm will compare issue title of a current row with one from a previous one.
        #It should not actually happen that two identical issue titles are in the file, however in case if there will be 2 similar in the row, only first one will be counted.
        #If it is, then a new will be used for an issue.
        goodPen=str.split(str(row[5]),",")
        goodWri=str.split(str(row[6]),",")
        goodCov=str.split(str(row[7]),",")
        for o in goodPen:
            for p in goodWri:
                for l in goodCov:
                    row[3] = row[3].replace('-', '') 
                    if row[3].strip() == "None":
                        date = None
                    else:
                        date = datetime.strptime(row[3], '%B %d, %Y')
                        date =  date.strftime('%Y-%m-%d')                    
                    val=(i,row[2],date,row[4],o,p,l,row[8],row[9],row[10],row[11])
                    mycursor.execute(sql,val)
                    mydb.commit()
        i+=1
print("Off I go then")

flag=False
j=1
i=1
sql="INSERT INTO comics (comic_id,issue_id) VALUES (%s,%s)"
for x,row in file.iterrows():
    #This now is prevous two parts combined and simplyfied as this only needs two atributes of a file to work with.
    #It generates an id for each comic series and an id for each issue, while also making sure that right issue belongs to right series and writing it in the database.
    #flag is used as starting position instead of a counter.
    if flag==True:
        if oldCom!=row[0]:
            i+=1
            oldCom=row[0]

        if oldIss!=row[2]:
            j+=1
            oldIss=row[2]

        val=(i,j)
        mycursor.execute(sql,val)
        mydb.commit()

    else:
        val=(i,j)
        oldCom=row[0]
        oldIss=row[2]
        flag=True          
        mycursor.execute(sql,val)
        mydb.commit()
print("Work,work")
