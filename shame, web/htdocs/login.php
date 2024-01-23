<html>
<head>
<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-F3w7mX95PdgyTmZZMECAngseQB83DfGTowi0iMjiWaeVhAn4FJkqJByhZMI3AhiU" crossorigin="anonymous">
<title>VELIKII SUP</title>
 <style>
input + span {
  padding-right: 30px;
}

input:invalid+span:after {
  position: absolute;
  content: '✖';
  padding-left: 5px;
}

input:valid+span:after {
  position: absolute;
  content: '✓';
  padding-left: 5px;
}
</style>
</head>
<body>
<br><br><br><br>
<div class="row text-center">
    <div class="col-4"></div>
    <div class="col-4">
        <div class="LabelBlock">
            <h1>LOGIN</h1>
        </div>
        <br>
        <form action="Main.php" method="POST" class="">
            Your code:
            <input type="text" id="code" name="Code" class="form-control" minlength="4" maxlength="4"  placeholder="****" required onchange= "testing()"><span class="validity"></span><br><br>
            <script>
              function testing(){
                alert("Wrong datatype");

                
                var cod = document.getElementById("code");
                
               if (var_dump(is_int(cod))==false){
                 alert("LOOL");

               };

                for(var i=0;i<=5;i++)
                {
                  if(value[i]!=0 || value[i]!=1 || value[i]!=2 || value[i]!=3 || value[i]!=4 || value[i]!=5 || value[i]!=6 || value[i]!=7  || value[i]!=8 || value[i]!=9)
                {
                 var a=a+1;
                }
              }
              if (a<4){alert("Only NUMBERS are valid");}
            }
            </script>
            Password:
            <input type="password" name="Pass" class="form-control" id="pass" style="margin-bottom: 3%" minlength="4" required maxlength="4" placeholder="****"><span class="validity"></span>
            <br>
            <p><br><button type="submit" class="btn btn-outline-dark">Log in</button></p>
        </form>
        
    </div>
</div>

<script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.3/dist/umd/popper.min.js" integrity="sha384-W8fXfP3gkOKtndU4JGtKDvXbO53Wy8SZCQHczT5FMiiqmQfUpWbYdTil/SxwZgAN" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/js/bootstrap.min.js" integrity="sha384-skAcpIdS7UcVUC05LJ9Dxay8AXcDYfBJqt1CJ85S/CFujBsIzCIv+l9liuYLaMQ/" crossorigin="anonymous"></script>
</body>
</html>