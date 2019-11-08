<?php
	$servername = "localhost";
	$username = "root";
	$password = "root";
	$db = "weatherfm";

	$conn = mysqli_connect($servername, $username, $password, $db);
	
	if (!$conn) {
		  die("Connection failed: " . mysqli_connect_error());
	}

	$sql = "UPDATE `audio` SET `audio_active` = '0' WHERE `id` = '1'";
	
	if (mysqli_query($conn, $sql)) {
    	echo "Record updated successfully";
	} else {
    	echo "Error updating record: " . mysqli_error($conn);
	}

	mysqli_close($conn);
?>