<html>
<meta http-equiv="refresh" content="15" />
<header>
	<?php
		require_once('../mysqli_connect.php');
		$query = "select ID from data";
		$result = mysqli_query($dbc,$query);
	?>
	<link rel="stylesheet" href="Show.css">
	 <script type="text/javascript" src="Show.js"></script>
	 <script type="text/javascript">
	 </script>
</header>
<body>
<div>
	<h1 margin = "auto">Vòng Tay</h1>
	<form class="cf" method = "get" action = "Show.php">
    <input type= "text" list = "wrist" name = "wrist" id="wrist-input">
	<datalist id = "wrist" onclick="myFunction()">
		<?php 
			while ($row = mysqli_fetch_array($result))
			{
		?>
		<option value = "<?php echo $row["ID"] ?>"><?php echo $row["ID"] ?></option>
		<?php 
			} 
			if (!$_GET)
			{
				$ID = HK1;
			}
			else 
			{
				$ID = $_GET["wrist"];
			}
			$query = "SELECT BEAT, SPO2, _LONG, LAT, TEMPERATURE from data where ID = '$ID'";
			$result = mysqli_query($dbc,$query);
			$row = mysqli_fetch_array($result);
		?>
	</datalist>
  <input type="submit" value="Submit" id="input-submit">
</form>
</div>
<div class="slideshow-container">
  <div class="mySlides fade">
    <div class="numbertext">1 / 3</div>
    <img src="images/Heart.png" class = "hinh">
    <div class="text"><?php echo $row["BEAT"]; ?></div>
  </div>

  <div class="mySlides fade">
    <div class="numbertext">2 / 3</div>
    <img src="images/Oxygen.png" class="hinh">
    <div class="text"><?php echo $row["SPO2"]; ?></div>
  </div>

  <div class="mySlides fade">
    <div class="numbertext">3 / 3</div>
    <img src="images/thermometer.png" class="hinh">
    <div class="text"><?php echo $row["TEMPERATURE"]; ?></div>
  </div>
  
  <div class="mySlides fade">
    <div class="numbertext">3 / 3</div>
    <img src="images/location.png" class="hinh">
    <div class="text"><?php echo $row["LAT"].' '.$row["_LONG"]; ?></div>
  </div>

  <a class="prev" onclick="plusSlides(-1)">&#10094;</a>
  <a class="next" onclick="plusSlides(1)">&#10095;</a>
</div>
<br>

<div style="text-align:center">
  <span class="dot" onclick="currentSlide(1)"></span> 
  <span class="dot" onclick="currentSlide(2)"></span> 
  <span class="dot" onclick="currentSlide(3)"></span> 
  <span class="dot" onclick="currentSlide(4)"></span>
</div>
</body>
</html>