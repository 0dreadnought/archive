#!C:\Users\Ильяс\AppData\Local\Programs\Python\Python311\python.exe

import cgi,cgitb
import mysql.connector
import time
import uuid

print("Content-type:text/html\n")

mac= hex(uuid.getnode())
now=time.ctime()

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
input=form.getvalue('in')
sum=0

if (name is None):
    print("<html><body><script>window.location.href='Login.html'</script>")

for num in str(input):
    sum=sum+int(num)




sql="INSERT INTO userhistory (user,MAC,task,inputs,results,time) VALUES (%s,%s,%s,%s,%s,%s)"
val=(name,mac,1,input,sum,now)
cur.execute(sql,val)
db.commit()
db.close()



action="""
<html>
<head>
<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-F3w7mX95PdgyTmZZMECAngseQB83DfGTowi0iMjiWaeVhAn4FJkqJByhZMI3AhiU" crossorigin="anonymous">

</head>
<body>
    <form action="history.py" method="POST">
        <button class="btn-outline-dark"type="submit">My history</button>
        <input type="text" id="historyname" name="username" style="display:none;">
    </form>
    <script>
            document.getElementById('historyname').value='{name}';
    </script>

<div style="position: relative;width: 100%;height: 100%;">
        
        <div style="position: absolute;left: 25%;top: 25%;height: 50%;width: 50%;font-size: larger;text-align: center;">
        Final sum is:{sum}

<script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.3/dist/umd/popper.min.js" integrity="sha384-W8fXfP3gkOKtndU4JGtKDvXbO53Wy8SZCQHczT5FMiiqmQfUpWbYdTil/SxwZgAN" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/js/bootstrap.min.js" integrity="sha384-skAcpIdS7UcVUC05LJ9Dxay8AXcDYfBJqt1CJ85S/CFujBsIzCIv+l9liuYLaMQ/" crossorigin="anonymous"></script>
</div></div>
</body>
</html>
""".format(name=name,sum=sum)

print(action)