<?php
echo"
<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-F3w7mX95PdgyTmZZMECAngseQB83DfGTowi0iMjiWaeVhAn4FJkqJByhZMI3AhiU' crossorigin='anonymous'>
";
echo"
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


</style>";
//connecting to the database
$name=$_POST["name"];
$surname=$_POST["surname"];
$link = mysqli_connect( "localhost", "root", "", "loginpass" )
    or die( "Error:" . mysqli_error( $link ) );

$query = "SELECT * FROM `students`";
$result = mysqli_query( $link, $query );
$count = 0;
//Checking if there is a student with given name and surname
while( $row = mysqli_fetch_array( $result ) ) {
    if( $row['Name'] == $name && $row['Surname'] == $surname ) {
        $count++;

        $link2 = mysqli_connect( "localhost", "root", "", "12e" )
    or die( "Error:" . mysqli_error( $link2 ) );

$student =$name.$surname;
        $query2 = "SELECT * FROM `$student`";
        $result2 = mysqli_query( $link2, $query2 );

        echo '<div class="row text-center"><div class="col-0"></div>';
        echo '<div class="col-1" padding="0px">';

        echo "<table class='Table'>";
        echo "<tr>
        <th>Number of lesson</th>
        <th>Time</th>
        <th>Mon</th>
        <th>Tue</th>
        <th>Wen</th>
        <th>Thu</th>
        <th>Fri</th>
        </tr>";

    while( $row2 = mysqli_fetch_array( $result2 ) ) {

        
            echo "<tr>";
            for($i=0;$i<=6;$i++)
            {
                echo "<td border='1'>".$row2[$i]."</td>";
            }
        echo "</tr>";
        
        
        }
        echo "</table>";
        echo "</div>";
    }
}




if( $count == 0 ) {
    echo "<br>";
echo "<br>";
echo "<br>";
echo "<br>";
echo "<br>";
echo "<center>";
echo "<h1>Incorrect Student's Name or Surname</h1>";
echo "</center>";
echo "<form action='returnmain.php' method='POST'>";
echo "<Br>";
echo "<p>";
echo "<center><Br>";
echo "<input type='submit' class='btn btn-outline-dark ' value='Try again'>";
echo "<center>";
echo "</p>";
echo "</form>";
}
echo "</body>";

?>