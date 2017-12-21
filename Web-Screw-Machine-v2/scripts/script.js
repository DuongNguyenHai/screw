$(document).ready(function() {

// Its really important to put the below variables at the begin of the file
var GMT = 7;
var collDay = "ScrewByDay";
var collHour = "ScrewByHour";
var collMinute = "ScrewByMinute";

// Get id of machine
var idMachine = indexMachine();
// set collection correspond with id of machine.
// collection may be "ScrewByDay_1" or "ScrewByDay_2" ...
initDataMachine();

////////////////////////////////////////////////////////////////////////////

var graphWorkingOpts = {
    tagId: "graphWorking",
    title: "Yield Rate graph by hour",
    seriesName: "Yield Rate",
    type: "line",
    collection: collHour,
    property: ["date", "total", "screw", "_id"],
    maxDocument: 48,
    cycleUpdate: 900000,
    index: 0,
    rangeSelector: [["hour", 12, '12h'], ["day", 1, 'd']],
    update: true,
    unit: "%"
}

var graphWorkingByDayOpts = {
    tagId: "graphWorkingByDay",
    title: "Yield Rate graph by day",
    seriesName: "Yield Rate",
    type: "line",
    collection: collDay,
    property: ["date", "total", "screw", "_id"],
    maxDocument: 90,
    index: 0,
    rangeSelector: [["week", 1, 'w'], ["month", 1, 'm']],
    update: false,
    unit: "%"
}

var graphFailByMinutesOpts = {
    tagId: "graphFailByMinutes",
    title: "Yield Rate graph by minute",
    seriesName: "Yield Rate",
    type: "line",
    collection: collMinute,
    property: ["date", "total", "screw", "_id"],
    rangeSelector: [["hour", 6, '6h'], ["hour", 12, '12h']],
    maxDocument: 96,
    index: 0,
    update: true,
    cycleUpdate: 900000,
    unit: "%"
}

var graphFailOpts = {
    tagId: "graphFail",
    title: "Failure graph by hour",
    collection: collHour,
    property: ["screw", "draft", "feeder", "fixture", "date"],
    seriesName: ['Screw', 'Draft', 'Feeder', 'Fixture'],
    typeTime: "hour",
    maxDocument: 48,
    update: true,
    cycleUpdate: 900000
}

var graphFailAllOpts = {
    tagId: "graphFailAll",
    title: "Failure graph by day",
    collection: collDay,
    property: ["screw", "draft", "feeder", "fixture", "date"],
    seriesName: ['Screw', 'Draft', 'Feeder', 'Fixture'],
    maxDocument: 90,
    typeTime: "day",
    update: false
}

var inforOpts = {
    collection: collDay,
    property: ["screw", "draft", "feeder"],
    objectChange: ["total fail","screw fail", "draft fail", "feeder fail", "fixture fail", "yield rate"],
    update: true,
    cycleUpdate: 5000,
    cycleUpdateTotalProduct: 30000
}

var screwPosistionOpts = {
    tagId: "graphScrewPosition",
    title: "Screw position failure by day",
    collection: collDay,
    property: ["screwPosition"],
    maxDocument: 1,
    seriesName: "screw failure",
    update: false
}

var graphPressureVacuumOpts = {
    tagId: "graphPressureVacuum",
    title: "Pressure of Vacuum by day",
    seriesName: "pressure",
    type: "line",
    collection: collHour,
    property: ["date", "vaccumPressure", "_id"],
    maxDocument: 168,
    index: 0,
    rangeSelector: [["week", 1, 'w'], ["month", 1, 'm']],
    update: false,
    unit: "kpa"
}

updateInforBox(inforOpts);

var graphFail = drawPieBarChart(graphFailOpts);
loadDataPieBarChart(graphFail, graphFailOpts);

var graphWorking = drawGraphYeildRate(graphWorkingOpts);
loadDataYeildRate(graphWorking, graphWorkingOpts);

// I wanna show some status of system
showCurrentInfor();

var oneTime = false;

$(".adv-option").on("click", function() {
    $(".adv-option-content").toggleClass("showup");
    $(this).toggleClass("btn-primary");
    $(".section").toggleClass("hidden");

    if(!oneTime) {
        var graphFailByMinutes = drawGraphYeildRate(graphFailByMinutesOpts);
        loadDataYeildRate(graphFailByMinutes, graphFailByMinutesOpts);

        var graphWorkingByDay = drawGraphYeildRate(graphWorkingByDayOpts);
        loadDataYeildRate(graphWorkingByDay, graphWorkingByDayOpts);

        var graphScrewPosition = drawPareto(screwPosistionOpts);
        loadScrewPosition(graphScrewPosition, screwPosistionOpts);

        var graphFailAll = drawPieBarChart(graphFailAllOpts);
        loadDataPieBarChart(graphFailAll, graphFailAllOpts);

        var graphPressureVacuum = drawGraphOneSerie(graphPressureVacuumOpts);
        loadDataOneSerie(graphPressureVacuum, graphPressureVacuumOpts);

        oneTime = true;
    }
});

function initDataMachine() {
    collDay += "_" + idMachine;
    collHour += "_" + idMachine;
    collMinute += "_" + idMachine;
}

function indexMachine() {
    return $("#body").attr('idMachine');
}

function showCurrentInfor() {
    var res = requestSimple("time");
    res.success(function (data) {
        console.log(data);
    });
    var res = requestForChart("infor", collDay);
}

function drawGraphYeildRate(graphOpts) {

    var graph = Highcharts.stockChart(graphOpts.tagId, {

        title: {
            text: graphOpts.title
        },
        chart: {
            events: {
                load: function() {
                    if(graphOpts.update) {
                        var ser = this.series[0];
                        setInterval(function () {
                            var res = requestForChart("chart", graphOpts.collection, graphOpts.property, "short", graphOpts.index);
                            res.success(function(data) {
                                if(data.length>2) {
                                    var cont = JSON.parse(data);
                                    for(j in cont) {
                                        var tt = (cont[i][1] == null) ? 0 : cont[i][1];
                                        var sc = (cont[i][2] == null) ? 0 : cont[i][2];
                                        var pers = (tt == 0) ? 100: (tt - sc)*100/tt;
                                        ser.addPoint(
                                            [
                                                parseInt(Number(cont[i][0].$date.$numberLong)),
                                                pers
                                            ], true, false);
                                    }
                                } else {}
                            });
                        }, graphOpts.cycleUpdate);
                    }
                }
            }
        },

        rangeSelector: {
            buttons: [{
                type: graphOpts.rangeSelector[0][0],
                count: graphOpts.rangeSelector[0][1],
                text: graphOpts.rangeSelector[0][2]
            }, {
                type: graphOpts.rangeSelector[1][0],
                count: graphOpts.rangeSelector[1][1],
                text: graphOpts.rangeSelector[1][2]
            }, {
                type: 'all',
                text: 'All'
            }],
            selected: 0
        },

        tooltip: {
            pointFormatter: function() {
                return '<span style="color:"></span>'+this.series.name+': <b>'+this.y.toFixed(2)+'</b> %<br/><br/>DPPM: <b>'+ Math.round((100-this.y)*10000)+'</b>';
            },
            valueDecimals: 2
            // pointFormat: '<span style="color:{series.color}">{series.name}</span>: <b>{point.y}</b> %<br/> <br/> DPPM: ',
        },

        plotOptions: {
            line: {
                dataLabels: {
                    enabled: true,
                    format: '{point.y:.2f} ' + graphOpts.unit
                },
            }
        },

        yAxis: {
            labels: {
                enabled: false
            }
        },

        xAxis: {
            type: 'datetime'
        },

        navigator: {
            enabled: false
        },

        series: [{
            name: graphOpts.seriesName,
            type: graphOpts.type,
            data: []
        }]

    });

    return graph;
}

function drawGraphOneSerie(graphOpts) {

    var graph = Highcharts.stockChart(graphOpts.tagId, {

        title: {
            text: graphOpts.title
        },
        chart: {
            events: {
                load: function() {
                    if(graphOpts.update) {
                    }
                }
            }
        },

        rangeSelector: {
            buttons: [{
                type: graphOpts.rangeSelector[0][0],
                count: graphOpts.rangeSelector[0][1],
                text: graphOpts.rangeSelector[0][2]
            }, {
                type: graphOpts.rangeSelector[1][0],
                count: graphOpts.rangeSelector[1][1],
                text: graphOpts.rangeSelector[1][2]
            }, {
                type: 'all',
                text: 'All'
            }],
            selected: 0
        },

        tooltip: {
            pointFormatter: function() {
                return '<span style="color:"></span>'+this.series.name+': <b>'+this.y.toFixed(2)+'</b>';
            },
            valueDecimals: 2
            // pointFormat: '<span style="color:{series.color}">{series.name}</span>: <b>{point.y}</b> %<br/> <br/> DPPM: ',
        },

        plotOptions: {
            line: {
                dataLabels: {
                    enabled: true,
                    format: '{point.y:.2f} ' + graphOpts.unit
                }
            }
        },

        yAxis: {
            labels: {
                enabled: false
            }
        },

        xAxis: {
            type: 'datetime'
        },

        navigator: {
            enabled: false
        },

        series: [{
            name: graphOpts.seriesName,
            type: graphOpts.type,
            data: []
        }]
    });

    return graph;
}

function requestSimple(requestType) {
    return $.post(
        "function/request.php",
        {
            request: requestType
        }
    );
}

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

function loadDataYeildRate(chart, opts) {

    var res = requestForChart("chart", opts.collection, opts.property, "rightside", opts.maxDocument);
    res.success(function (data) {

        var cont = JSON.parse(data);
        var point = [];
        opts.index = cont[cont.length-1][opts.property.length-1];
        for(i in cont) {
            var tt = (cont[i][1] == null) ? 0 : cont[i][1];
            var sc = (cont[i][2] == null) ? 0 : cont[i][2];
            var pers = (tt == 0) ? 100: (tt - sc)*100/tt;
            point.push(
                [
                    parseInt(Number(cont[i][0].$date.$numberLong)),
                    pers
                ]
            );
        }

        chart.update({
            series: [{
                data: point
            }]
        });
    });
}

function loadDataOneSerie(chart, opts) {

    var res = requestForChart("chart", opts.collection, opts.property, "rightside", opts.maxDocument);
    res.success(function (data) {
        var cont = JSON.parse(data);
        var point = [];
        opts.index = cont[cont.length-1][opts.property.length-1];
        // console.log(cont);
        for(i in cont) {
            if((cont[i][1] == null)==false) {
                point.push(
                    [
                        parseInt(Number(cont[i][0].$date.$numberLong)),
                        cont[i][1]
                    ]
                );
            }
        }
        // console.log(point);
        chart.update({
            series: [{
                data: point
            }]
        });
    });
}
function round2decimal(num) {
    return +(Math.round(num + "e+2")  + "e-2");
}

function drawPieBarChart(graphOpts) {
    var graph = Highcharts.chart(graphOpts.tagId, {
        title: {
            text: graphOpts.title
        },
        chart: {
            events: {
                load: function() {
                    if(graphOpts.update) {
                        setInterval(function () {
                            loadDataPieBarChart(graph, graphOpts);
                        }, graphOpts.cycleUpdate);
                    }
                }
            }
        },

        xAxis: {
            scrollbar: {
                enabled: true
            }
        },

        yAxis: {
            title: {
                enabled: false
            },
            labels: {
                enabled: false
            }
            // visible: false
        },

        labels: {
            items: [{
            }]
        },

        plotOptions: {
            spline: {
                dataLabels: {
                    enabled: true,
                    format: '{point.y}'
                },
                enableMouseTracking: false
            }
        },

        series: [{
            type: 'column',
            name: graphOpts.seriesName[0]
        }, {
            type: 'column',
            name: graphOpts.seriesName[1]
        }, {
            type: 'column',
            name: graphOpts.seriesName[2]
        },
        {
            type: 'column',
            name: graphOpts.seriesName[3]
        },
        {
            type: 'spline',
            name: 'Total',
            zIndex: 10,
            marker: {
                lineWidth: 2,
                lineColor: Highcharts.getOptions().colors[3],
                fillColor: 'white'
            }
        },
        {
            type: 'pie',
            tooltip: {
                headerFormat: '',
                pointFormat: '<span style="color:{point.color}">{point.name}</span>: <b>{point.percentage:.1f}%</b>'
            },
            data: [{
                name: graphOpts.seriesName[0],
                color: Highcharts.getOptions().colors[0]
            }, {
                name: graphOpts.seriesName[1],
                color: Highcharts.getOptions().colors[1]
            }, {
                name: graphOpts.seriesName[2],
                color: Highcharts.getOptions().colors[2]
            }, {
                name: graphOpts.seriesName[3],
                color: Highcharts.getOptions().colors[3]
            }],
            center: [15, 00],
            size: 60,
            showInLegend: false,
            dataLabels: {
                enabled: false
                // format: '{point.name}: {point.y:.1f}%'
            }
        }

        ]
    });

    return graph;
}

function loadDataPieBarChart(chart, graphOpts) {

    var res = requestForChart("chart", graphOpts.collection, graphOpts.property, "rightside", graphOpts.maxDocument);
    res.success(function (data) {

        var cont = JSON.parse(data);

        var point = [[], [], [], [], [], []];
        var pp = new Array(4);
        pp.fill(0);

        for(i in cont) {
            var dd = new Date(parseInt(Number(cont[i][4].$date.$numberLong)) - GMT*3600*1000);  // convert to local time
            var date;
            if(graphOpts.typeTime=="day") {
                date = dd.getFullYear() + "-" + (dd.getMonth()+1) + "-" + dd.getDate();
            } else if(graphOpts.typeTime=="hour") {
                date = dd.getDate() + "/" + (dd.getMonth()+1) + "<br/>" + (dd.getHours()<10? "0":"") + dd.getHours() + ":" +  (dd.getMinutes()<10? "0":"") + dd.getMinutes();
            }

            var sc = (cont[i][0] == null) ? 0 : cont[i][0];
            var dr1 = (cont[i][1] == null) || (typeof(cont[i][1]["1"]) == 'undefined') ? 0 : cont[i][1]["1"];
            var dr2 = (cont[i][1] == null) || (typeof(cont[i][1]["2"]) == 'undefined') ? 0 : cont[i][1]["2"];
            var fd = (cont[i][2] == null) ? 0 : cont[i][2];
            var fx = (cont[i][3] == null) ? 0 : cont[i][3];

            point[0].push([ sc ]);
            point[1].push([ dr1 + dr2 ]);
            point[2].push([ fd ]);
            point[3].push([ fx ]);
            point[4].push([ sc + dr1 + dr2  + fd + fx ]);
            point[5].push([ date ]);

            pp[0] += sc;
            pp[1] += dr1 + dr2;
            pp[2] += fd;
            pp[3] += fx;
        }


        var pie = new Array(4);
        pie.fill(0);
        if(pp[0] + pp[1] + pp[2] + pp[3]) {
            pie[0] = parseFloat( ((pp[0]) *100/(pp[0] + pp[1] + pp[2] + pp[3])).toFixed(2) );
            pie[1] = parseFloat( ((pp[1]) *100/(pp[0] + pp[1] + pp[2] + pp[3])).toFixed(2) );
            pie[2] = parseFloat( ((pp[2]) *100/(pp[0] + pp[1] + pp[2] + pp[3])).toFixed(2) );
            pie[3] = parseFloat( ((pp[3]) *100/(pp[0] + pp[1] + pp[2] + pp[3])).toFixed(2) );
        }

        chart.update({
            xAxis: {
                min: (cont.length-7-1) < 0 ? 0 : cont.length-7,
                max: (cont.length-1) < 0 ? 0 : cont.length-1 ,
                categories: point[5]
            },
            series: [
                {
                    data: point[0]
                }, {
                    data: point[1]
                }, {
                    data: point[2]
                }, {
                    data: point[3]
                }, {
                    data: point[4]
                },
                {
                    data: [
                        {
                            name: graphOpts.seriesName[0],
                            y: pie[0]
                        }, {
                            name: graphOpts.seriesName[1],
                            y: pie[1]
                        }, {
                            name: graphOpts.seriesName[2],
                            y: pie[2]
                        }, {
                            name: graphOpts.seriesName[3],
                            y: pie[3]
                        }
                    ]
                }
            ]
        });
    });
}

function updateInforBox(opts) {
    setInterval(function() {
        var res = requestForChart("infor", opts.collection);
        res.success(function (data) {
            var cont = JSON.parse(data);
            var sc = (typeof(cont.screw) == 'undefined') ? 0 : cont.screw;
            var dr1 = (typeof(cont.draft) == 'undefined') || (typeof(cont.draft["1"]) == 'undefined') ? 0 : cont.draft["1"];
            var dr2 = (typeof(cont.draft) == 'undefined') || (typeof(cont.draft["2"]) == 'undefined') ? 0 : cont.draft["2"];
            var fd = (typeof(cont.feeder) == 'undefined') ? 0 : cont.feeder;
            var fx = (typeof(cont.fixture) == 'undefined') ? 0 : cont.fixture;
            var pers = (typeof(cont.total) == 'undefined') || cont.total == 0 ? 100 : (cont.total - sc)*100/cont.total;

            $(".infor-content[object='"+ opts.objectChange[0] +"']").find(".infor-val").html(sc + dr1 + dr2 + fd + fx);
            $(".infor-content[object='"+ opts.objectChange[1] +"']").find(".infor-val").html(sc);
            $(".infor-content[object='"+ opts.objectChange[2] +"']").find(".infor-val").html(dr1 + dr2);
            $(".infor-content[object='"+ opts.objectChange[3] +"']").find(".infor-val").html(fd);
            $(".infor-content[object='"+ opts.objectChange[4] +"']").find(".infor-val").html(fx);
            $(".infor-content[object='"+ opts.objectChange[5] +"']").find(".infor-val").html(pers.toFixed(2));
        });
    }, opts.cycleUpdate);

    setInterval(function() {
        var res = requestForChart("infor", opts.collection);
        res.success(function (data) {
            var cont = JSON.parse(data);
            var total = (typeof(cont.total) == 'undefined') || cont.total == 0 ? 0 : cont.total;
            $(".total-val").html(total/4);
        });
    }, opts.cycleUpdateTotalProduct);


}

function drawPareto(graphOpts) {
    var graph = Highcharts.chart('graphScrewPosition', {
        chart: {
            renderTo: 'graphScrewPosition',
            type: 'column'
        },
        title: {
            text: graphOpts.title
        },
        xAxis: {

        },

        yAxis: [{
            title: {
                text: ''
            }
        }, {
            title: {
                text: ''
            },
            minPadding: 0,
            maxPadding: 0,
            max: 100,
            min: 0,
            opposite: true,
            labels: {
                format: "{value}%"
            }
        }],
        series: [{
            type: 'pareto',
            name: 'Pareto',
            yAxis: 1,
            zIndex: 10,
            baseSeries: 1
        }, {
            name: graphOpts.seriesName,
            type: 'column',
            zIndex: 2
        }]
    });

    return graph;
}

function loadScrewPosition(chart, opts) {

    var res = requestForChart("chart", opts.collection, opts.property, "rightside", opts.maxDocument);
    res.success(function (data) {

        var cont = JSON.parse(data);
        var point = new Array(16);
        point.fill(0);

        for(i in cont) {
            for (var j = 1; j < point.length+1; j++) {
                var ct = (cont[i]["0"] == null) || (typeof(cont[i]["0"][""+ j +""]) == 'undefined') ? 0 : cont[i]["0"][""+ j +""];
                point[j-1] += ct;
            }
        }

        var column = point.map(function(e,i){return {ind: i, val: e}});
        // sort index/value couples, based on values
        column.sort(function(x, y){return x.val > y.val ? -1 : x.val == y.val ? 0 : 1});
        // make list keeping only indices
        var indices = column.map(function(e){return e.ind + 1});
        var dt = column.map(function(e){return e.val});
        chart.update({
            xAxis: {
                categories: indices
            },
            series: [
            {

            },
            {
                data:  dt
            }]
        });
    });
}


});
