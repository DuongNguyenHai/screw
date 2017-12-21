<?php

include 'function/communicate.php';
include 'function/layout.php';

// the data of machine 2 insdie screw1 collection and
// the data of machine 1 insdie screw2 collection and
$doc1 = getLastDocument($conn, "ScrewByDay_1");
$sr1 = is_null($doc1->screw) ? 0 : $doc1->screw;
$yr1 = is_null($doc1->total) ? 0 : ($doc1->total - $sr1)*100/$doc1->total;
$yr1 = sprintf('%0.2f', $yr1);

$doc2 = getLastDocument($conn, "ScrewByDay_2");
$sr2 = is_null($doc2->screw) ? 0 : $doc2->screw;
$yr2 = is_null($doc2->total) ? 0 : ($doc2->total - $sr2)*100/$doc2->total;
$yr2 = sprintf('%0.2f', $yr2);

?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
    <title>Screw Machine</title>

    <!-- JS -->
    <script type="text/javascript" src="scripts/jquery/jquery.js"></script>
    <script type="text/javascript" src="scripts/jquery/jquery-ui.min.js"></script>
    <script type="text/javascript" src="scripts/jquery.ui.touch-punch.min.js"></script>
    <script type="text/javascript" src="scripts/main.js"></script>
    <!-- CSS -->
    <!-- <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Open+Sans"> -->
    <link rel="stylesheet" type="text/css" href="fonts/font-awesome/css/font-awesome.min.css">

    <!-- Bootstrap -->
    <link rel="stylesheet" type="text/css" href="css/bootstrap.min.css">
    <link rel="stylesheet" type="text/css" href="css/jquery-ui.min.css">

    <link rel="stylesheet" type="text/css" href="css/main.css">
    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
      <script src="https://oss.maxcdn.com/html5shiv/3.7.3/html5shiv.min.js"></script>
      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->
</head>
<body>
    <section class="sec-machine">
        <div class="container">
            <div class="row">
                <div class="col-xs-12">
                    <div class="box-title">
                        <h3>Smart Screw Machine</h3>
                    </div>
                </div>
                <div class="box-machine">
                    <h4>Machine</h4>
                    <a href="screw1.php">1</a>
                    <span class="box-machine-val"><i id="screw-machine-val-1"><?php echo $yr1; ?></i><i> %</i></span>
                </div>
                <div class="box-machine">
                    <h4>Machine</h4>
                    <a href="screw2.php">2</a>
                    <span class="box-machine-val"><i id="screw-machine-val-2"><?php echo $yr2; ?></i><i> %</i></span>
                </div>
            </div>
        </div>
    </section>
</body>
</html>
