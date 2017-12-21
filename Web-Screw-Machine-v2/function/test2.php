<?php

include 'database.php';

// $doc = getLastDocument($conn, collTempt);
// $lamp = getDocument($conn, collSetup, "title", "lamp");
//
// var_dump($lamp);

$collection = $conn->ScrewByDay_1;

// $start = new MongoDate(strtotime("2010-01-15 00:00:00"));
// $end = new MongoDate(strtotime("2010-01-30 00:00:00"));
$start = new ISODate("2012-11-21T00:00:00+01:00");

$query = array( "date" => array( '$gt' => $start) ); //note the single quotes around '$gt'
$cursor = $collection->find( $query );

foreach ($cursor as $doc) {
    var_dump($doc);
}

?>
