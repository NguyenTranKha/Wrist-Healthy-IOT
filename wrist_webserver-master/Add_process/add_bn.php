<?php
	require_once('../mysqli_connect.php');
	
	$user_android = $_POST["user_android"];
	$wrist = $_POST["wrist"];
	
	$query = "INSERT INTO relative values('$wrist', '$user_android')";
	$result = mysqli_query($dbc,$query);
	
	if ($result)
		{
			//phpAlert("Success!");
			header('Location: ' . $_SERVER['HTTP_REFERER']);
		}
	else 
		echo "failed";
?>