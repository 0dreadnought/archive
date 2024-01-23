#!C:\Users\Ильяс\AppData\Local\Programs\Python\Python311\python.exe

import cgi,cgitb
import mysql.connector
import time
import random

print("Content-type:text/html\n")

db= mysql.connector.connect(
    host="localhost",
    user="root",
    password="",
    database="PackUK"
)
cur=db.cursor(buffered=True)

sql=("INSERT INTO bookings(fname,sname,depar,dest,type,date,refernumber) VALUES(%s,%s,%s,%s,%s,%s,%s)")

cgitb.enable()
form= cgi.FieldStorage()

depar= form.getvalue("from") 
dest= form.getvalue("to")

depardate= form.getvalue("todate")
returndate= form.getvalue("returndate")

deparvehicle=form.getvalue("deparvehicle")
returnvehicle=form.getvalue("returnvehicle")

passengers= form.getvalue("passengers")

page="""<html>
<head>
    <title>PackUK</title>
</head>
<body>
        <div style="position: relative;width: 100%;height: 100%;">
        <div style="zoom:1.6; display:block; position: absolute;left: 15%;top: 20%;width: 70%;font-size: larger;">
"""
page2="""
</body></html>
"""
print(page)

for i in range(0,int(passengers)):
    name=str(form.getvalue("f"+str(i)))
    sname=str(form.getvalue("s"+str(i)))
    num=str(int(time.time())) + str(random.randint(1, 1000000))+ str(random.randint(1, 10000))
    print("""<fieldset style="width:auto; margin-bottom:20px"><legend>Passenger """+name+""" """+sname+"""</legend>
    <div style="display:flex; justify-content:space-around;"><span>Departure-"""+depar+"""</span><span>Destination-"""+dest+"""</span></div><br>
    <div style="display:flex; justify-content:space-around;"><span>Date-"""+depardate+"""</span><span>Vehicle-"""+deparvehicle+"""</span></div><br>
    Reference Number="""+num+"""
    </fieldset> """)
    val=(name,sname,depar,dest,deparvehicle,depardate,num)
    cur.execute(sql,val)
    db.commit()

if returndate != "None":
    for i in range(0,int(passengers)):
        name=str(form.getvalue("f"+str(i)))
        sname=str(form.getvalue("s"+str(i)))
        num=str(int(time.time())) + str(random.randint(1, 1000000)) +str(random.randint(1, 10000))
        print("""<fieldset style="width:auto; margin-bottom:20px"><legend>Passenger """+name+""" """+sname+"""</legend>
        <div style="display:flex; justify-content:space-around;"><span>Departure-"""+dest+"""</span><span>Destination-"""+depar+"""</span></div><br>
        <div style="display:flex; justify-content:space-around;"><span>Date-"""+returndate+"""</span><span>Vehicle-"""+returnvehicle+"""</span></div><br>
        Reference Number="""+num+"""
        </fieldset> """)
        val=(name,sname,dest,depar,returnvehicle,returndate,num)
        cur.execute(sql,val)
        db.commit()
print(page2)