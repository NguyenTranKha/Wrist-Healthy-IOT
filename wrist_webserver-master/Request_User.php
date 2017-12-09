<?php
		require_once('mysqli_connect.php');
	
		$ID = $_GET["ID_RELATIVE"];	
		$query = "Select * from user_android where ID_RELATIVE = '$ID'";
	
		$result = mysqli_query($dbc,$query);
		
		$myObj = new stdClass();
		
		while ($row = mysqli_fetch_row($result))
		{
			if ($row[0] != null)
			{
				
				$ID_ANDROID = $row[0];
				$ID = $row[1];
				$PHONE = $row[2];
				$ADDRESS = $row[3];
				$Current_ADDR = $row[4];
				
				$jsonresult[] = array("ID_ANDROID" => $ID_ANDROID, "ID" => $ID, "PHONE" => $PHONE, "ADDRESS" => $ADDRESS, "Location" => $Current_ADDR);
			}
		}
		header('Content-type:application/json');
		echo json_encode($jsonresult);
?>