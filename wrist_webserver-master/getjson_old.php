<!DOCTYPE html>
<html>
<head>
	<?php
		require_once('mysqli_connect.php');
	
		$query = "Select * from data";
	
		$result = mysqli_query($dbc,$query);
	
	?>
</head>
<body>
	<script type="text/javascript">
	
		<?php
		while ($row = mysqli_fetch_row($result))
		{ ?>
			var obj = new Object();

			obj.ID = <?php echo $row[0] ?>;
			obj.temperature = <?php echo $row[1] ?>;
			obj.humidity = <?php echo $row[2] ?>;
			obj.state = <?php echo $row[3] ?>;
			//var jsonString = JSON.stringify(obj);
			//document.write(jsonString);
			document.write("abcd");
		<?php } ?>
	</script>
</body>
</html>