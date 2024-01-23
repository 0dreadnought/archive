#!C:\Users\Ильяс\AppData\Local\Programs\Python\Python311\python.exe

import cgi,cgitb
import mysql.connector

print("Content-type:text/html\n")

db= mysql.connector.connect(
    host="localhost",
    user="root",
    password="",
    database="packuk"
)

cur= db.cursor()

cgitb.enable()
form= cgi.FieldStorage()
num=form.getvalue("rnumber")

sql="SELECT * FROM bookings WHERE refernumber='{num}'".format(num=num)
cur.execute(sql)
ticket=cur.fetchall()   



db.commit()
db.close()
if len(ticket)>0:
    
    i=0
    for row in ticket:
        for item in row:
            
            if i==0:
                fname=item
            elif i==1:
                sname=item
            elif i==2:
                depar=item
            elif i==3:
                dest=item
            elif i==4:
                vehicle=item
            elif i==5:
                date=item
            i+=1
        break   

    action1="""
    <html>
    <head>
        <title>PackUK</title>
    </head>
    <body>
    <div style="position: relative;width: 100%;height: 100%;">
            
            <div style="position: absolute;left: 25%;top: 25%;height: 50%;width: 50%;font-size: larger;text-align: center;">
        <fieldset style="width:auto; margin-bottom:20px"><legend>Passenger """+fname+""" """+sname+"""</legend>
        <div style="display:flex; justify-content:space-around;"><span>Departure-"""+depar+"""</span><span>Destination-"""+dest+"""</span></div><br>
        <div style="display:flex; justify-content:space-around;"><span>Date-"""+date+"""</span><span>Vehicle-"""+vehicle+"""</span></div><br>
        Reference Number="""+num+"""
        </fieldset></body></html>"""




    print(action1)
else:print("""<html>
    <head>
        <title>PackUK</title>
    </head>
    <body>
    <div style="position: relative;width: 100%;height: 100%;">
            
            <div style="position: absolute;left: 25%;top: 25%;height: 50%;width: 50%;font-size: larger;text-align: center;"><fieldset>Sorry, nothing found :-(</fieldset>""")

