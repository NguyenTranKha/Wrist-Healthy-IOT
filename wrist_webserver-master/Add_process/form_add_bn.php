<!DOCTYPE html>
<html lang="en" >
<head>
  <meta charset="UTF-8">
  <title>Thêm người thân</title>
  
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/meyer-reset/2.0/reset.min.css">

  
      <link rel="stylesheet" href="css/style.css">
<?php
	require_once('../mysqli_connect.php');
?>
  
</head>

<body>
  <h1>Thêm người thân</h1>
<form class="cf" method = "post" action = "add_bn.php">
  <div class="half left cf">
	<h1>Người Dùng Android</h1>
    <input list = "user_android" name = "user_android">
	<datalist id = "user_android">
		<?php 
			$query = "select ID_RELATIVE from user_android";
			$result = mysqli_query($dbc,$query);
			while ($row = mysqli_fetch_array($result))
			{
		?>
	<option value = "<?php echo $row["ID_RELATIVE"]; ?>"><?php echo $row["ID_RELATIVE"]; ?></option>
		<?php } ?>
	</datalist>
  </div>
  <div class="half right cf">
    <h1>Vòng Tay</h1>
    <input list = "wrist" name = "wrist">
	<datalist id = "wrist">
		<?php
			$query = "select ID from data";
		$result = mysqli_query($dbc,$query);
		while ($row = mysqli_fetch_array($result))
		{
		?>
	<option value = "<?php echo $row["ID"] ?>"><?php echo $row["ID"]; ?></option>
		<?php } ?>
	</datalist>
  </div>  
  <input type="submit" value="Submit" id="input-submit">
</form>
  
  
</body>
</html>