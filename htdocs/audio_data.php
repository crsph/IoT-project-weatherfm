<?php
	header("Content-Type: application/json");
	$servername = "localhost";
	$username = "root";
	$password = "root";
	$db = "weatherfm";

	// Create connection
	$conn = mysqli_connect($servername, $username, $password, $db);
	
	// Check connection
	if (!$conn) {
		  die("Connection failed: " . mysqli_connect_error());
	}

	$result = $conn->query("SELECT * FROM audio");
	
	$dbdata = array();

	while($row = $result->fetch_assoc()) {
		$dbdata[]=$row;
	}

	$myJSON = json_encode($dbdata);

	echo $myJSON;
	mysqli_close($conn);
?>