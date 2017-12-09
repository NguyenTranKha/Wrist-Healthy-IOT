<?php
	
	require_once('mysqli_connect.php');

	$UserName = $_POST["usernamesignup"];
	$Password = $_POST["passwordsignup"];
	$Email = $_POST["emailsignup"];
	$Password_Confirm = $_POST["passwordsignup_confirm"];
	
	if ($Password == $Password_Confirm)
	{
		$query = "INSERT INTO user_android(ID_RELATIVE, PASSWORD) values('$UserName','$Password')";
		$result = mysqli_query($dbc,$query);
		
		if ($result)
		{
			phpAlert("Signup Successfully!");
		}
	}
?>