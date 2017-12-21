<?php

require 'vendor/autoload.php'; // include Composer's autoloader

define("dbName","ScrewMachine");

$MAX_VAL = 365;	// 3 month

function connectDatabase($databaseName) {
	$client = new MongoDB\Client("mongodb://localhost:27017");
	return $client->$databaseName;
}

function getSchedule($conn, $collName, $type) {
	$coll = $conn->$collName;
	$document = $coll->findOne(["title"=>$type]);
	$schedule = iterator_to_array($document->schedule, true);
	return $schedule;
}

function getDocument($conn, $collName, $key_select, $val_select) {
	$coll = $conn->$collName;
	$doc = $coll->findOne([$key_select=>$val_select]);
	return $doc;
}

function getField($conn, $collName, $key_select, $val_select, $key_get) {
	$coll = $conn->$collName;
	$doc = $coll->findOne([$key_select=>$val_select]);
	return $doc->$key_get;
}

function getLastDocument($conn, $collName) {
	$coll = $conn->$collName;
	$cursor = $coll->find([], [
        'limit' => 1,
        'sort' => ['$natural' => -1],
    ]);

	foreach($cursor as $document) {
		$doc = $document;
	}

    return $doc;
}

function updateField($conn, $collName, $key_select, $val_select, $key_change, $val_change) {
	$coll = $conn->$collName;
	$updateResult = $coll->updateOne(
		[$key_select => $val_select],
		['$set'=>[$key_change => $val_change]]
	);
}


function getFieldArray($conn, $coll, $type, $property, $index)
{
	global $MAX_VAL;
	$tt = getField($conn, $coll, "_id", "id", "total");
	$total = intval($tt);
	$start = 0;
	$count = 0;
	$arr = array();

	if($type === "total") {
		if($total > $MAX_VAL) {
			$start = $total- $MAX_VAL;

		}
	}
	else if($type === "short") {
		$start = $index;
	} else if($type === "rightside") {
		$start = ($total - $index) > 0 ? ($total - $index) : 0;
	}

	for ($i = $start; $i < $total; $i++) {
		$doc = getDocument($conn, $coll, "_id", $i);
		if($doc) {
			for ($j=0; $j < count($property); $j++) {
				$arr[$count][$j] = $doc[$property[$j]];
			}
			$count++;
		}
	}
	return $arr;
}

function getFieldLastTime($conn, $coll, $property, $lastTime) {

}

$GLOBALS["conn"] = connectDatabase(dbName);

?>
