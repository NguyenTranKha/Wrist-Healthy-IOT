<?php
		require_once('mysqli_connect.php');
	
		$ID = $_GET["ID"];	
		$query = "Select * from data where ID = '$ID'";
	
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
			
				//$myJSON = json_encode($myObj);
			
				//echo $myJSON;
			}
		}
		header('Content-type:application/json');
		echo json_encode($jsonresult);
?>