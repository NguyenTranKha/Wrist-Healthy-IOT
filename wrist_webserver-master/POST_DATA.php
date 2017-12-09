
<?php
	require_once('mysqli_connect.php');
	$ID = $_POST["ID"];
	$temperature = $_POST["temperature"];
	$SPO2 = $_POST["SPO2"];
	$BEAT = $_POST["BEAT"];
	$state = 0;
	$LONG = $_POST["LONG"];
	$LAT = $_POST["LAT"];
	
	$query = "UPDATE data SET temperature = $temperature, SPO2 = $SPO2, BEAT = $BEAT, _LONG = $LONG, LAT = $LAT where ID='$ID'";
	echo $query.'\n';
	
	$result = mysqli_query($dbc,$query);
	if ($result)
	{
		echo 'UPDATED\n';
	}
	
	if ($BEAT < 60) 
	{
		$query = "UPDATE data set HEART_STATE = 'TIM DAP QUA CHAM', STATE = 1 where ID='$ID'";
	}
	else if ($BEAT > 100) 
	{
		$query = "UPDATE data set HEART_STATE_STATE = 'TIM DAP NHANH', STATE = 1 where ID='$ID'";
	}
	else if (($BEAT >= 60) && ($BEAT <= 100))
	{
		$query = "UPDATE data set HEART_STATE = 'TIM DAP BINH THUONG', STATE = 0 where ID='$ID'";
	}
	
	echo $query.'\n';
	
	$result = mysqli_query($dbc,$query);
	if ($result)
	{
		echo 'UPDATED\n';
	}
	
    if (($SPO2 >= 88) && ($SPO2 <= 100))
	{
		$query = "UPDATE data set LUNG_STATE = 'OXY BINH THUONG', STATE = 0 where ID='$ID'";
	}
	else if (($SPO2 < 88))
	{
		$query = "UPDATE data set LUNG_STATE = 'OXY QUA THAP', STATE = 1 where ID='$ID'";
	}
	echo $query.'\n';
	
	$result = mysqli_query($dbc,$query);
	if ($result)
	{
		echo 'UPDATED\n';
	}
?>

<script type="text/javascript">
</script>



