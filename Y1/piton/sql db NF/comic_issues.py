import mysql.connector
import pandas
from datetime import datetime
mydb= mysql.connector.connect(
    host="localhost",
    user="root",
    password="",
    database="marvel"
)
mycursor=mydb.cursor()

file = pandas.read_csv("Marvel_Comics.csv")
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
