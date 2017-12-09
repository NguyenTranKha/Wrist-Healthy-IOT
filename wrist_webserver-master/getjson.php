	<?php
		require_once('mysqli_connect.php');
	
		$query = "Select * from data";
	
		$result = mysqli_query($dbc,$query);
		
		$myObj = new stdClass();
		while ($row = mysqli_fetch_row($result))
		{
			if ($row[0] != null)
			{
				
				/* $myObj->ID = $row[0];
				$myObj->temperature = $row[1];
				$myObj->SPO2 = $row[2];
				$myObj->BEAT = $row[3];
				$myObj->state = $row[4]; */
				
				$ID = $row[0];
				$temperature = $row[1];
				$SPO2 = $row[2];
				$BEAT = $row[3];
				$state = $row[4];
				
				$jsonresult[] = array("ID" => $ID, "temperature" => $temperature, "SPO2" => $SPO2, "BEAT" => $BEAT, "state" => $state);
			
				//$myJSON = json_encode($myObj);
			
				//echo $myJSON;
			}
		}
		header('Content-type:application/json');
		echo json_encode($jsonresult);
	?>
