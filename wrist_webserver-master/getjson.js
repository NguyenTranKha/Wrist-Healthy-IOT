
<?php
	require_once('mysqli_connect.php');
	
	$query = "Select * from data";
	
	$result = mysqli_query($dbc,$query);
	
?>


var obj = new Object();
var jsonString;
	
	<?php
	while ($row = mysqli_fetch_row($result))
	{
		echo $row[0].$row[1]?>
	
	obj.ID = <?php Print($row[0]); ?>
	obj.temperature = <?php Print($row[1]); ?>
	obj.humidity = <?php Print($row[2]); ?>
	obj.state = <?php Print($row[3]); ?>
	jsonString = JSON.stringify(obj);
	document.write(jsonString);
	document.write("go");
	<?php } ?>
