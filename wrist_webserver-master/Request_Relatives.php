<?php
		require_once('mysqli_connect.php');
	
		$ID = $_GET["ID_RELATIVE"];	
		$query = "Select B.ID, B.TEMPERATURE, B.SPO2, B.BEAT, B.STATE, B._LONG, B.LAT , B.HEART_STATE, B.LUNG_STATE  from relative A, data B where A.ID_RELATIVE = '$ID' and A.ID = B.ID";
	
		$result = mysqli_query($dbc,$query);
		
		$myObj = new stdClass();
		
		while ($row = mysqli_fetch_row($result))
		{
			if ($row[0] != null)
			{
				
				$ID = $row[0];
				$temperature = $row[1];
				$SPO2 = $row[2];
				$BEAT = $row[3];
				$state = $row[4];
				$LONG = $row[5];
				$LAT = $row[6];
				$HEART_STATE = $row[7];
				$LUNG_STATE = $row[8];
				
				$jsonresult[] = array("ID" => $ID, "temperature" => $temperature, "SPO2" => $SPO2, "BEAT" => $BEAT, "state" => $state, "LONG" => $LONG, "LAT" => $LAT, "HEART_STATE" => $HEART_STATE, "LUNG_STATE" => $LUNG_STATE);
			}
		}
		header('Content-type:application/json');
		echo json_encode($jsonresult);
?>