<?php
echo "<head>
<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-F3w7mX95PdgyTmZZMECAngseQB83DfGTowi0iMjiWaeVhAn4FJkqJByhZMI3AhiU' crossorigin='anonymous'>
<title>CHOOSE YOUR CHAMPION</title>
<style>
 td,th {
   border: 1px solid;
 }
.LoginBlock {
    border: solid 2px black;
    width: 200px;
    margin: 5px;
    padding: 5px;
   
}

.Table{
    margin: 5px;
    padding: 5px;
    width: 500%;
    }

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
<body>";
echo "<script>
function testing(){
  alert('Wrong datatype');}</script>";
  //Connecting to database
$i=-1;
$code=$_POST["Code"];
$pass=$_POST["Pass"];
$link = mysqli_connect( "localhost", "root", "", "loginpass" )
    or die( "Error:" . mysqli_error( $link ) );

$query = "SELECT * FROM `datab`";
$result = mysqli_query( $link, $query );

$count = 0;

 // Code and password verification
while( $row = mysqli_fetch_array( $result ) ) {
    if( $row['Code'] == $code && $row['Pass'] == $pass ) {
        $count++;
        echo '<div class="row text-center"><div class="col-0"></div>';
echo '<div class="col-1" padding="0px">';
    
        echo "<table class='Table'>";
echo "<tr>
<th>Name</th>
<th>Surname</th>
<th>Year of birth</th>
<th>№</th>
</tr>";
$query2 = "SELECT * FROM `students`";
$result2 = mysqli_query( $link, $query2 );
while ($row =$result2-> fetch_assoc())
{
echo "<tr>";
foreach ($row as $col_value)
{
echo "<td border='1'>$col_value</td>";
}
echo "</tr>";


}

echo "</table>";

echo '</div>';



echo '<div class="col-2" style="margin-left: 410px">';

echo "<div class='LoginBlock'>";
echo "<form action= 'Student.php' method= 'POST'>";
echo "<p> Name of student :<input type='text' name='name' required class='form-control' minlength='5' maxlength='12'  placeholder='******' onchange='testing()'><span class='validity'></span><Br> Surname of student:
<input type='text' name='surname' class='form-control' required minlength='5' maxlength='12'  placeholder='******' onchange='testing()'><span class='validity'></span><Br><Br>";
echo "<input type = 'submit' class='btn btn-outline-dark ' value= 'Submit'>";
echo "</form>";
echo "</div>";
echo '</div>';      
echo "</div>";
        break;
    }
}

if( $count == 0 ) {
    echo '<div class="row text-center">';
    echo "<div class='col-12'>";
    echo "<br>";
echo "<br>";
echo "<br>";
echo "<br>";
echo "<br>";

echo "<h1>Incorrect Code or Password</h1>";

echo "<form action='login.php' method='POST'>";
echo "<Br>";
echo "<p>";

echo "<input type='submit' class='btn btn-outline-dark ' value='Try again'>";

echo "</p>";
echo "</form></div></div>";
}
echo "</body>";
?>