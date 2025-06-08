import mysql.connector
import pandas
mydb= mysql.connector.connect(
    host="localhost",
    user="root",
    password="",
    database="marvel"
)
flag=False
j=1
i=1
mycursor= mydb.cursor()
sql="INSERT INTO comics (comic_id,issue_id) VALUES (%s,%s)"

file = pandas.read_csv("Marvel_Comics.csv")
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
