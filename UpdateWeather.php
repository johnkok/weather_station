<?php

$url = $_SERVER['QUERY_STRING'];
$servername = "localhost";
$username = "my_username";
$password = "My_password";
$dbname = "my_db";

	
	function updWeather() {
	
	   parse_str($GLOBALS['url']);

	   if ($ws1 != '' && $ws2 != '' && $ws3 != '' && $ws4 != '' && $ws5 != '' && 
	       $ws6 != '' && $ws7 != '' && $ws8 != '' && $ws9 != '' && $ws0 != '') {
	              
	       mysqli_query( $GLOBALS['conn'], "INSERT INTO `weather` (`in_temp`, `out_temp`, `in_hum`, `out_hum`, `wind_speed`, `wind_dir`, `wind_gust`, `pressure`, `rain`, `light`) VALUES ('$ws1', '$ws2', '$ws3', '$ws4', '$ws5', '$ws6', '$ws7', '$ws8', '$ws9', '$ws0')");
	      
	   }
	}


	// Create connection
	$conn = mysqli_connect($servername, $username, $password, $dbname);
	   
	// Check connection
	if (!$conn) {
	   die("Connection failed: " . mysqli_connect_error()); 
	}
	
	updWeather();
	
	$conn->close();
	
?>
