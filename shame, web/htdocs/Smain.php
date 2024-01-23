<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <link href='https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-F3w7mX95PdgyTmZZMECAngseQB83DfGTowi0iMjiWaeVhAn4FJkqJByhZMI3AhiU' crossorigin='anonymous'>
</head>
<body>
    <?php
        $i=-1;
        $code=$_POST["Code"];
        $pass=$_POST["Pass"];
        $link = mysqli_connect( "localhost", "root", "", "loginpass" )
            or die( "Error:" . mysqli_error( $link ) );
        
        $query = "SELECT * FROM `datab`";
        $result = mysqli_query( $link, $query );
        
        $count = 0;
          echo '<table rules="none" bordercolor="white"><tr bordercolor="white"><td bordercolor="white">';
        while( $row = mysqli_fetch_array( $result ) ) {
            if( $row['Code'] == $code && $row['Pass'] == $pass ) {
                $count++;
                
        
                echo "<table>";
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
        
        
         echo "</td><td bordercolor='white'>";
        
        
        echo "<div class='LoginBlock'>";
        echo "<form action= 'Student.php' method= 'POST'>";
        echo "<p> Name of student :<input type='text' name='name'><Br> Surname of student:
        <input type='text' name='surname'><Br>";
        echo "<input type = 'submit' value= 'Submit'>";
        echo "</form>";
        echo "</div>";
        echo "</td></tr>";
        echo "</table>";
        
                break;
            }
        }
        
        if( $count == 0 ) {
            echo "<br>";
        echo "<br>";
        echo "<br>";
        echo "<br>";
        echo "<br>";
        echo "<center>";
        echo "Incorrect Code or Password";
        echo "</center>";
        echo "<form action='login.php' method='POST'>";
        echo "<Br>";
        echo "<p>";
        echo "<center>";
        echo "<input type='submit' value='Try again'>";
        echo "<center>";
        echo "</p>";
        echo "</form>";
        }
        echo "</body>";
    ?>

    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.3/dist/umd/popper.min.js" integrity="sha384-W8fXfP3gkOKtndU4JGtKDvXbO53Wy8SZCQHczT5FMiiqmQfUpWbYdTil/SxwZgAN" crossorigin="anonymous"></script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/js/bootstrap.min.js" integrity="sha384-skAcpIdS7UcVUC05LJ9Dxay8AXcDYfBJqt1CJ85S/CFujBsIzCIv+l9liuYLaMQ/" crossorigin="anonymous"></script>
</body>
</html>