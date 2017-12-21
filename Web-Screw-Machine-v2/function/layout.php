<?php 

include 'database.php';

function console_log($data) {
    echo '<script>';
    echo 'console.log('. json_encode( $data ) .')';
    echo '</script>';
}

function second2hour($ss) {
    return $ss/3600;
}

function second2minute($ss) {
    return $ss/60;
}

function objectInfor($name, $value, $unit) {
    echo '<div class="col-xs-6 col-sm-4 col-md-2 border-right">
            <div class="infor-content" object="'.strtolower($name).'" clearfix">
                <span class="infor-name">'.$name.'</span>
                <div class="infor-content-body">
                    <span class="infor-val">'.$value.'</span>
                    <span class="infor-unit"><em>'.$unit.'</em></span>
                </div>
            </div>
        </div>';
}

function printSectionInfor($conn, $coll) {

    echo '<section class="sec-infor-current">
        <div class="container">
            <div class="row">
                <div class="col-xs-12">
                    <div class="dashboard-title"> 
                        <h3>Monitor by day</h3>
                        <div class="title-underline"></div>
                    </div>
                    <div class="infor box-shadow clearfix">';

                    $doc = getLastDocument($conn, $coll);
                    $tt = $sr = $dr = $fe = $fx = $yr = 0;
                    if($doc->_id === "id") {

                    }
                    else {
                        $sr = is_null($doc->screw) ? 0 : $doc->screw;
                        $dr = is_null($doc->draft["1"]) &&  is_null($doc->draft["2"]) ? 0 : $doc->draft["1"] + $doc->draft["2"];
                        $fe = is_null($doc->feeder) ? 0 : $doc->feeder;
                        $fx = is_null($doc->fixture) ? 0 : $doc->fixture;
                        $tt = $sr + $dr + $fe + $fx;
                        $yr = is_null($doc->total) ? 0 : ($doc->total - $sr)*100/$doc->total;
                        $yr = sprintf('%0.2f', $yr);
                    }
                    
                    objectInfor("Total fail", $tt, "");
                    objectInfor("Screw fail", $sr, "");
                    objectInfor("Draft fail", $dr, "");
                    objectInfor("Feeder fail", $fe, "");
                    objectInfor("Fixture fail", $fx, "");
                    objectInfor("Yield rate", $yr, "%");
                    
                    echo '</div>
                </div>
            </div>
        </div>
    </section>';

}

?>