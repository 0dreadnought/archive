import mysql.connector
import pandas
mydb= mysql.connector.connect(
    host="localhost",
    user="root",
    password="",
    database="marvel"
)
file = pandas.read_csv("Marvel_Comics.csv")
i=1 #The comic books are pre-sorted in alphabetic order in the csv file.
    #This means that no comic will randomly occure in the midle of the table, after it was once.
    #By knowing that a simple counter can be set as an ID for all the comic series
mycursor= mydb.cursor()
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
