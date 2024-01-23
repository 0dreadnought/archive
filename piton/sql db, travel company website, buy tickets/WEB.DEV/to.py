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
cur=db.cursor()

cgitb.enable()
form= cgi.FieldStorage()

depar= form.getvalue("from") 
dest= form.getvalue("to")

depardate= form.getvalue("todate")
returndate= form.getvalue("return")

bus= bool(form.getvalue("checkBuses"))
train= bool(form.getvalue("checkTrains"))
plane= bool(form.getvalue("checkPlanes"))
adults= form.getvalue("adult")
children= form.getvalue("child")
infants= form.getvalue("infant")
if(int(adults)>int(infants)):
    payingpassengers=int(adults)+int(children)
else:payingpassengers=int(adults)+int(children)+int(infants)

if (bus==True):
    if (train==True):
        if (plane==True):
            vehicles="AND type IN (1,2,3)"
        else:vehicles="AND type IN (1,2)"
    else: vehicles="AND type=1"
elif (train==True):
        if (plane==True):
             vehicles=" AND type IN (2,3)"
        else:vehicles="AND type=2"
elif (plane==True):
    vehicles="AND type=3"
else: vehicles= "AND type IN (1,2,3)"

dow= datetime.strptime(depardate,'%Y-%m-%d').weekday()
sql="SELECT* FROM trips WHERE depar='{depar}' AND dest='{dest}' AND dow='{dow}' {vehicles}".format(depar=depar,dest=dest,dow=dow,vehicles=vehicles)
cur.execute(sql)
trips= cur.fetchall()

page1="""
<html><head>
    <title>PackUK</title>
<style>
.highlighted{
background-color:#f2f2f2;
}
</style>
<script>

function changetotal(){
    const t = document.querySelector('input[name=" Trip"]:checked');
    const r = document.querySelector('input[name=" Return"]:checked');
    var value1=0;
    var value2=0;
    if (t != null){
    value1 = parseInt(t.getAttribute("data-price"), 10);
    }
    if (r!=null)  {
    value2 = parseInt(r.getAttribute("data-price"), 10);
    }
      
      var result = value1 + value2;

      result= """+str(payingpassengers)+""" * result;
      document.getElementById("total").innerHTML=result;
    
}
</script><script>
function highlightRow(button){
var row=button.closest("tr");
var table=row.closest("table");
var prevRow=table.querySelector(".highlighted");
if (prevRow){
prevRow.classList.remove("highlighted");
}
row.classList.add("highlighted")
}

</script>

</head>
<body><div style="position: relative;width: 100%;height: 100%;">
        <div style="position: absolute;left: 25%;top: 25%;height: 50%;width: 50%;font-size: larger;text-align: center;">
        <form action="buyingphase.py" method="POST">"""

page2="""

<br>
<span>Total:</span>
&pound<span id="total">0</span>
<button>Continue</button>
<div style="display:none">  

<input type="text" name="from" id="from">
<input type="text" name="to" id="to">
<input type="text" name="todate" id="todate">
<input type="text" name="returndate" id="returndate">
<input type="text" name="vehicles" id="vehicles">
<input type="text" name="adult" id="adult">
<input type="text" name="child" id="child">
<input type="text" name="infant" id="infant">

<script>
document.getElementById("from").value='{depar}';
document.getElementById("to").value='{dest}';
document.getElementById("todate").value='{depardate}';
document.getElementById("returndate").value='{returndate}';
document.getElementById("vehicles").value='{vehicles}';
document.getElementById("adult").value='{adults}';
document.getElementById("child").value='{children}';
document.getElementById("infant").value='{infants}';
</script>

</div>
</form>
</div></div>
</body>
</html>
""".format(depar=depar,dest=dest,depardate=depardate,returndate=returndate,vehicles=vehicles,adults=adults,children=children,infants=infants)
print(page1)


def table(td,tr,departing):
    print("""<fieldset><legend>"""+tr+""" Options """+str(departing)+"""</legend><table width="100%">
        <tr><td>Departure</td><td>Destination</td><td>Day of the Week</td><td>Price[&pound]</td><td>Journey Time[hrs]</td><td>Vehicle</td>""")
    counter = 0
    results = 0
    for row in trips:
        for item in row:
            if counter % 6 == 0:  
                results+=1
                print("</tr><tr>")
            print("<td><label for=' "+td+str(results)+"'>")
            if(counter%6==2):
                if item==0:
                    item="Monday"
                elif item==1:
                    item="Tuesday"
                elif item==2:
                    item="Wednesday"
                elif item==3:
                    item="Thursday"
                elif item==4:
                    item="Friday"
                elif item==5:
                    item="Saturday"
                else: item="Sunday"
            if(counter%6==5):
                
                if(item==1):item="Bus"
                elif(item==2):item="Train"
                else:item="Plane"
            if(counter%6==3):
                print("""<input style="display:none;" type="radio" required onchange="changetotal();" onclick="highlightRow(this)" data-price='"""+str(item)+"""' id=" """+td+str(results)+"""" name=" """+tr+ """" value='"""+str(results)+"""'>""")
            print(item)
            print("</label></td>")
            counter+=1
    print("</tr></table></fieldset>")
        
    print("<br>")

if len(trips) >0:
    table("t","Trip",depardate)
else: print("<fieldset><legend>Trip Options</legend>Nothing found :-(</fieldset>")
    
if returndate != None:
    returning=datetime.strptime(returndate,'%Y-%m-%d')
    dow=returning.weekday()
    sql="SELECT* FROM trips WHERE depar='{dest}' AND dest='{depar}' AND dow='{dow}' {vehicles}".format(depar=depar,dest=dest,dow=dow,vehicles=vehicles)
    cur.execute(sql)
    trips= cur.fetchall()
    if len(trips) > 0:
        table("r","Return",returndate)
    else: print("<fieldset><legend>Return Options</legend>Sorry, nothing found :-(</fieldset>")
        

print(page2)