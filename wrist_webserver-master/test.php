<!DOCTYPE html>
<html>
	<head>
	<body>
		<?php $php_demo = "hello" ?>
		
		<script type="text/javascript">
			var jsDemo = "<?php echo $php_demo ?>";
			document.write(jsDemo);
		</script>
	</body>
</html>