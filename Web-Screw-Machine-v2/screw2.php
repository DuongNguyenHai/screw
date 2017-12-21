<?php

include 'function/communicate.php';
include 'function/layout.php';

$machine = "screw-machine";
$idMachine = "2";

$collDay = "ScrewByDay_" . $idMachine;
$doc = getLastDocument($conn, $collDay);
$total = is_null($doc->total) ? 0 : $doc->total;

?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
    <title>Screw Machine 2</title>

    <!-- JS -->
    <script src="scripts/highchart/highchart.js"></script>
    <script src="scripts/highstock/highstock.js"></script>
    <script src="scripts/highchart/modules/series-label.js"></script>
    <script src="scripts/highchart/modules/pareto.js"></script>
    <script src="scripts/highchart/modules/exporting.js"></script>
    <script src="scripts/highchart/themes/grid-light.js"></script>

    <script type="text/javascript" src="scripts/jquery/jquery.js"></script>
    <script type="text/javascript" src="scripts/jquery/jquery-ui.min.js"></script>
    <script type="text/javascript" src="scripts/jquery.ui.touch-punch.min.js"></script>
    <script type="text/javascript" src="scripts/script.js"></script>

    <!-- CSS -->
    <!-- <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Open+Sans"> -->
    <link rel="stylesheet" type="text/css" href="fonts/font-awesome/css/font-awesome.min.css">

    <!-- Bootstrap -->
    <link rel="stylesheet" type="text/css" href="css/bootstrap.min.css">
    <link rel="stylesheet" type="text/css" href="css/jquery-ui.min.css">
    <link rel="stylesheet" type="text/css" href="css/highcharts.css">
    <link rel="stylesheet" type="text/css" href="css/index.css">
    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
      <script src="https://oss.maxcdn.com/html5shiv/3.7.3/html5shiv.min.js"></script>
      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->
</head>
<body id="body" class=<?php echo $machine; ?>  idMachine=<?php echo $idMachine; ?>>
    <nav>
        <div class="container">
            <div class="nav">
                <h1>Screw Machine 2</h1>
                <div class="option">
                    <div class="total-block"><span class="total-val"><?php echo $total/4; ?></span></div>
                    <button type="button" class="btn adv-option btn-primary">Advanced Monitor</button>
                </div>
            </div>
        </div>
    </nav>
    <section class="adv-option-content">
        <div class="container">
            <div class="row">
                <div class="col-lg-6">
                    <div class="row">
                        <div class="col-xs-12">
                            <div class="graph">
                                <div id="graphFailByMinutes" style="height:300px"></div>
                            </div>
                        </div>
                        <div class="col-xs-12">
                            <div class="graph">
                                <div id="graphWorkingByDay" style="height:300px;"></div>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="col-lg-6">
                    <div class="row">
                        <div class="col-xs-12">
                            <div class="graph">
                                <div id="graphScrewPosition" style="height:300px"></div>
                            </div>
                        </div>
                        <div class="col-xs-12">
                            <div class="graph">
                                <div id="graphFailAll" style="height:300px;"></div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="row">
                <div class="col-xs-12">
                    <div class="graph">
                        <div id="graphPressureVacuum" style="height:300px"></div>
                    </div>
                </div>
            </div>
        </div>
    </section>
    <section class="section">
        <?php
            printSectionInfor($conn, $collDay);
        ?>
    </section>
    <section class="sec-graph section">
        <div class="container">
            <div class="row">
                <div class="col-lg-6">
                    <div class="graph">
                        <div id="graphFail" style="height:400px;"></div>
                    </div>
                </div>
                <div class="col-lg-6">
                    <div class="graph">
                        <div id="graphWorking" style="height:400px;"></div>
                    </div>
                </div>
            </div>
        </div>
    </section>
    <div class="switch-machine">
        <a href="screw1.php" class=<?php if($idMachine==="1") echo "active"; ?> ><span>1</span></a>
        <a href="screw2.php" class=<?php if($idMachine==="2") echo "active"; ?> ><span>2</span></a>
    </div>
</body>
</html>
