#!C:\Users\Ильяс\AppData\Local\Programs\Python\Python311\python.exe

import cgi,cgitb
import mysql.connector
from datetime import datetime


print("Content-type:text/html\n")

db= mysql.connector.connect(
    host="localhost",
    user="root",
    password="",
    database="PackUK"
)
cur=db.cursor(buffered=True)

cgitb.enable()
form= cgi.FieldStorage()

depar= form.getvalue("from") 
dest= form.getvalue("to")

depardate= form.getvalue("todate")
returndate= form.getvalue("returndate")

vehicles=form.getvalue("vehicles")

adults= form.getvalue("adult")
children= form.getvalue("child")
infants= form.getvalue("infant")
passengers= int(adults)+int(children)+int(infants)
if(int(adults)>int(infants)):
    payingpassengers=int(adults)+int(children)
else:payingpassengers=int(adults)+int(children)+int(infants)

pickedtrip=form.getvalue(" Trip")
pickedreturn= form.getvalue(" Return")

dowd=datetime.strptime(depardate,'%Y-%m-%d').weekday()

sql="SELECT* FROM trips WHERE depar='{depar}' AND dest='{dest}' AND dow='{dowd}' {vehicles}".format(depar=depar,dest=dest,dowd=dowd,vehicles=vehicles)
cur.execute(sql)
for i in range(0,int(pickedtrip)):
    trips= cur.fetchone()
total=int(trips[3])*payingpassengers
deparvehicle=int(trips[5])

if(deparvehicle==1):
    deparvehicle="Bus"
elif(deparvehicle==2):
    deparvehicle="Train"
else:deparvehicle="Plane"

returnvehicle=None

if returndate != "None":
    dowr= datetime.strptime(returndate,'%Y-%m-%d').weekday()
    sql= "SELECT* FROM trips WHERE depar='{dest}' AND dest='{depar}' AND dow='{dowr}' {vehicles}".format(depar=depar,dest=dest,dowr=dowr,vehicles=vehicles)
    cur.execute(sql)
    for j in range(0,int(pickedreturn)):
        trips= cur.fetchone()
    total=total+int(trips[3])*payingpassengers
    returnvehicle=int(trips[5])
    if(returnvehicle==1):
        returnvehicle="Bus"
    elif(returnvehicle==2):
        returnvehicle="Train"
    else:returnvehicle="Plane"

page="""<html>
<head>
    <title>PackUK</title>
<script>
function checkcard(){
  var input = document.getElementById("card").value;
  const digits = input.split('').reverse().map(Number);
  let sum = 0;
  for (let i = 0; i < digits.length; i++) {
    let digit = digits[i];
    if (i % 2 === 1) {
      digit *= 2;
      if (digit > 9) {
        digit -= 9;
      }
    }
    sum += digit;
  }
  if (sum%10==0){
  }
  else{
  alert("Invalid card number");
  document.getElementById("card").value="";
  }
}
</script>
</head>
<body>
        <div style="position: relative;width: 100%;height: 100%;">
        <div style="zoom:1.6; display:flex; position: absolute;left: 15%;top: 20%;width: auto;font-size: larger;"><div><form id="form" action="finish.py"><fieldset style="border:0;"><legend><b>Payment</b><legend></fieldset>
        <div style="display:none">
        <input type="text" name="from" id="from">
        <input type="text" name="to" id="to">
        <input type="text" name="todate" id="todate">
        <input type="text" name="returndate" id="returndate">
        <input type="text" name="deparvehicle" id="deparvehicle">
        <input type="text" name="returnvehicle" id="returnvehicle">
        <input type="text" name="passengers" id="passengers">

<script>
document.getElementById("from").value='"""+str(depar)+"""';
document.getElementById("to").value='"""+str(dest)+"""';
document.getElementById("todate").value='"""+str(depardate)+"""';
document.getElementById("returndate").value='"""+str(returndate)+"""';
document.getElementById("deparvehicle").value='"""+str(deparvehicle)+"""';
document.getElementById("returnvehicle").value='"""+str(returnvehicle)+"""';
document.getElementById("passengers").value='"""+str(passengers)+"""';
</script></div>

"""

page2="""
<fieldset><legend>Card Details</legend>
<legend style="font-size:large">Card number</legend><input autocomplete="off" type="text" id="card" onchange="checkcard()"><br>
<span style="font-size:large">Month</span>    <span style="font-size:large">Year</span><br>
<input autocomplete="off" type="text" style="width:30px">   /   <input autocomplete="off" type="text"  style="width:60px"><br>
<legend style="font-size:large">Name on card</legend><input autocomplete="off" type="text"><br>
<legend style="font-size:large">Card security code</legend><legend style="font-size:small"><sup>Last 3 digits on the back of the card</sup></legend><input autocomplete="off" type="text" style="width:30px">
</fieldset>
</form></div><div style="margin-top:6.4%"><fieldset><legend>Total</legend>&pound{total}</fieldset><button form="form" style="width:100%">Continue</button></div>
""".format(total=total)        
        
        
print(page)
for i in range(0,passengers):
    print("""<fieldset style="margin-bottom:10px"><legend>Passenger """+str(i+1)+"""</legend>
    <input type="text" required placeholder="First name" name='f"""+str(i)+"""'>  <input type="text" placeholder="Midle name">  <input type="text" required name='s"""+str(i)+"""' placeholder="Second name"><br>
    <input type="text" placeholder="Document type">  <input type="text" placeholder="document number">
    <input type="text" placeholder="Nationality">  <legend style="font-size:large"><sub>Date of birth</sub></legend><input type="date">  
    </fieldset>""")
print(page2)