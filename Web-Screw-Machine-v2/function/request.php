<?php

include 'communicate.php';
include 'database.php';

function console_log($data) {
    echo '<script>';
    echo 'console.log('. json_encode( $data ) .')';
    echo '</script>';
}

if($_REQUEST["request"] === "time") {
	echo date("Y-m-d H:i:sa");
}
else if($_REQUEST["request"] === "infor") {
	$doc = getLastDocument($conn, $_REQUEST["collection"]);
	echo json_encode($doc, true);
}
else if($_REQUEST["request"] === "chart") {
    if($_REQUEST["type"] === "lastTime") {
    	// $arr = getFieldLastTime($conn, $_REQUEST["collection"], $_REQUEST["name"], $_REQUEST["index"]);
    	// echo json_encode($arr, true);
    } else {
        $arr = getFieldArray($conn, $_REQUEST["collection"], $_REQUEST["type"], $_REQUEST["property"], $_REQUEST["index"]);
    	echo json_encode($arr, true);
    }
}
else if($_REQUEST["request"] === "status") {
	$val = getField($conn, $_REQUEST["collection"], "title", $_REQUEST["name"], $_REQUEST["key"]);
	echo $_REQUEST["name"].":".($val?1:0);
}

?>
