#!C:\Users\Ильяс\AppData\Local\Programs\Python\Python311\python.exe
print("Content-type:text/html\n")
import cgi,cgitb
cgitb.enable()
form= cgi.FieldStorage()
import random
import mysql.connector
import time
import uuid


mac= hex(uuid.getnode())
now=time.ctime()

db= mysql.connector.connect(
    host="localhost",
    user="root",
    password="",
    database="users"
)
cur= db.cursor()

name=form.getvalue('username')

if (name is None):
    print("<html><body><script>window.location.href='Login.html'</script>")
    
action="""
<html>
<head>
<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-F3w7mX95PdgyTmZZMECAngseQB83DfGTowi0iMjiWaeVhAn4FJkqJByhZMI3AhiU" crossorigin="anonymous">

</head>
<body>
    <form action="history.py" method="POST">
        <button class="btn-outline-dark"type="submit">My history</button>
        <input type="text" id="historyname" name="username" style="visibility: hidden;">
    </form>
    <script>
            document.getElementById('historyname').value='{name}';
    </script>
<div style="position: relative;width: 100%;height: 100%;">
        
        <div style="position: absolute;left: 25%;top: 25%;height: 50%;width: 50%;font-size: larger;text-align: center;">
""".format(name=name)

print(action)

def main():
    random.seed()
    result=0
    max=form.getvalue("max")
    rolls=form.getvalue("rolls")

    for i in range(int(rolls)):
        num=random.randrange(1,int(max)+1)
        print(num)
        result=result+num
    print("<br>Final sum is:"+ str(result))
    input=str(max) +";"+ str(rolls)
    
    sql="INSERT INTO userhistory (user,MAC,task,inputs,results,time) VALUES (%s,%s,%s,%s,%s,%s)"
    val=(name,mac,2,input,result,now)
    cur.execute(sql,val)
    db.commit()
    db.close()

print("""<script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.3/dist/umd/popper.min.js" integrity="sha384-W8fXfP3gkOKtndU4JGtKDvXbO53Wy8SZCQHczT5FMiiqmQfUpWbYdTil/SxwZgAN" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/js/bootstrap.min.js" integrity="sha384-skAcpIdS7UcVUC05LJ9Dxay8AXcDYfBJqt1CJ85S/CFujBsIzCIv+l9liuYLaMQ/" crossorigin="anonymous"></script>
</div</div>
</body>
</html>
""")


if __name__ == "__main__":
    main()
    
