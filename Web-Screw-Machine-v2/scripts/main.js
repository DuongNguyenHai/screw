$(document).ready(function() {


setInterval(function() {
	var res = requestForChart("infor", "ScrewByDay_1");
    res.success(function (data) {
        var cont = JSON.parse(data);
        var sc = (typeof(cont.screw) == 'undefined') ? 0 : cont.screw;
        var pers = (typeof(cont.total) == 'undefined') || cont.total == 0 ? 100 : (cont.total - sc)*100/cont.total;
        $("#screw-machine-val-1").html(pers.toFixed(2));
    });

    var res2 = requestForChart("infor", "ScrewByDay_2");
    res2.success(function (data) {
        var cont = JSON.parse(data);
        var sc = (typeof(cont.screw) == 'undefined') ? 0 : cont.screw;
        var pers = (typeof(cont.total) == 'undefined') || cont.total == 0 ? 100 : (cont.total - sc)*100/cont.total;
        $("#screw-machine-val-2").html(pers.toFixed(2));
    });


}, 60000);

function requestForChart(requestType, coll, property, type, idx = 0) {
    return $.post(
        "function/request.php",
        {
            request: requestType,
            collection: coll,
            property: property,
            type: type,
            index: idx
        }
    );
}


});
