#!C:\Users\Ильяс\AppData\Local\Programs\Python\Python311\python.exe

import cgi,cgitb
import mysql.connector

print("Content-type:text/html\n")

db= mysql.connector.connect(
    host="localhost",
    user="root",
    password="",
    database="users"
)

cur= db.cursor()

cgitb.enable()
form= cgi.FieldStorage()
name=form.getvalue('username')

if (name is None):
    print("<html><body><script>window.location.href='Login.html'</script>")

sql="SELECT * FROM userhistory WHERE user='{name}'".format(name=name)
cur.execute(sql)
history=cur.fetchall()   

db.commit()
db.close()


action1="""
<html>
<head>
<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-F3w7mX95PdgyTmZZMECAngseQB83DfGTowi0iMjiWaeVhAn4FJkqJByhZMI3AhiU" crossorigin="anonymous">

</head>
<body>
<div style="position: relative;width: 100%;height: 100%;">
        
        <div style="position: absolute;left: 25%;top: 25%;height: 50%;width: 50%;font-size: larger;text-align: center;">
        <table width="100%">
        <tr><td>Username</td><td>MAC adress</td><td>Task number</td><td>Input</td><td>Result</td><td>Date</td></tr>
        <tr>
        """

action2="""</tr></table>
<script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.3/dist/umd/popper.min.js" integrity="sha384-W8fXfP3gkOKtndU4JGtKDvXbO53Wy8SZCQHczT5FMiiqmQfUpWbYdTil/SxwZgAN" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/js/bootstrap.min.js" integrity="sha384-skAcpIdS7UcVUC05LJ9Dxay8AXcDYfBJqt1CJ85S/CFujBsIzCIv+l9liuYLaMQ/" crossorigin="anonymous"></script>
</div></div>
</body>
</html>
"""

print(action1)
counter = 0
for row in history:
    for item in row:
        if counter % 6 == 0:  
            print("</tr><tr>")
        print("<td>")
        print(item)
        print("</td>")
        counter+=1
print(action2)