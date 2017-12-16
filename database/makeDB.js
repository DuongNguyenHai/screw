var ScrewByMinute_1 = "ScrewByMinute_1";
var ScrewByHour_1 = "ScrewByHour_1";
var ScrewByDay_1 = "ScrewByDay_1";

var ScrewByMinute_2 = "ScrewByMinute_2";
var ScrewByHour_2 = "ScrewByHour_2";
var ScrewByDay_2 = "ScrewByDay_2";

db = connect("localhost:27017");
db = db.getSiblingDB("ScrewMachine");
if(db)
	db.dropDatabase();

var GMT = 0;
var day = 7*24*3600*1000;
var localDate = getTime();

function getTime() {
	return new Date( new Date().getTime() + GMT * 3600 * 1000 - day);
}

function getTimeExpand(tm, expand) {
	return new Date( tm.getTime() + GMT * 3600 * 1000 + 1000*expand);
}

function hour2second(hour) {
	return hour*3600;
}

var totalDay = 2;
var totalHour = totalDay*24;
var totalMinute = totalHour*4;

db[ScrewByDay_1].insert({_id: "id", total: NumberInt(totalDay), infor: "log everything by day of machine 1", date: localDate});
db[ScrewByHour_1].insert({_id: "id", total: NumberInt(totalHour), infor: "Log everything by hour of machine 1", date: localDate});
db[ScrewByMinute_1].insert({_id: "id", total: NumberInt(totalMinute), infor: "Log everything by 15 minute of machine 1", date: localDate});

db[ScrewByDay_2].insert({_id: "id", total: NumberInt(totalDay), infor: "log everything by day of machine 2", date: localDate});
db[ScrewByHour_2].insert({_id: "id", total: NumberInt(totalHour-2), infor: "Log everything by hour of machine 2", date: localDate});
db[ScrewByMinute_2].insert({_id: "id", total: NumberInt(totalMinute), infor: "Log everything by 15 minute of machine 2", date: localDate});


// ScrewByDay_1
// structer
// {
// 	_id: number,
//	total: the total of screw
// 	screw: the number of screw failure,
// 	draft: the number of draft failure,
// 	feeder: the number of feeder failure,
// 	fixture: the number of fixture failure,
// 	screwPosition: [[position of screw, the number of failure], [position of screw, the number of failure]],
// 	date: DateTime
// }


for (var i = 0; i < totalDay; i++) {
	var tt = (2500 + (Math.floor(Math.random()*500) - 250)) * 16;
	var fl = (50 + (Math.floor(Math.random()*30) - 15));

	db[ScrewByDay_1].insert({
		_id: NumberInt(i),
		total: NumberInt(tt),
		screw: NumberInt(fl),
		draft: {
			1: NumberInt(15 + (Math.floor(Math.random()*6) - 3)),
			2: NumberInt(15 + (Math.floor(Math.random()*6) - 3))
		},
		feeder: NumberInt(30 + (Math.floor(Math.random()*20) - 10)),
		fixture: NumberInt(2 + (Math.floor(Math.random()*4) - 2)),
		screwPosition: {
			1: NumberInt(1), 2: NumberInt(2), 3: NumberInt(3), 4: NumberInt(4),
			5: NumberInt(5), 6: NumberInt(6), 7: NumberInt(7), 8: NumberInt(8),
			9: NumberInt(9), 10: NumberInt(10), 11: NumberInt(11), 12: NumberInt(12),
			13: NumberInt(13), 14: NumberInt(14), 15: NumberInt(15), 16: NumberInt(16)
		},
		vaccumPressure: [-45],
		date:  getTimeExpand(localDate, 86400*i)
	});

}

// ScrewByHour_1
// structer
// {
// 	_id: number,
//	total: the total of screw
// 	screw: the number of screw failure,
// 	draft: the number of draft failure,
// 	feeder: the number of feeder failure,
// 	fixture: the number of fixture failure,
// 	screwPosition: [[position of screw, the number of failure], [position of screw, the number of failure]],
// 	date: DateTime
// }


for (var i = 0; i < totalHour; i++) {
	var tt = (250 + (Math.floor(Math.random()*50) - 24)) * 16;
	var fl = (50 + (Math.floor(Math.random()*30) - 15));

	db[ScrewByHour_1].insert({
		_id: NumberInt(i),
		total: NumberInt(tt),
		screw: NumberInt(fl),
		draft: {
			1: NumberInt(15 + (Math.floor(Math.random()*6) - 3)),
			2: NumberInt(15 + (Math.floor(Math.random()*6) - 3))
		},
		feeder: NumberInt(30 + (Math.floor(Math.random()*20) - 10)),
		fixture: NumberInt(2 + (Math.floor(Math.random()*4) - 2)),
		screwPosition: {
			1: NumberInt(2), 2: NumberInt(1), 3: NumberInt(3), 4: NumberInt(4),
			5: NumberInt(5), 6: NumberInt(6), 7: NumberInt(8), 8: NumberInt(7),
			9: NumberInt(9), 10: NumberInt(10), 11: NumberInt(4), 12: NumberInt(12),
			13: NumberInt(13), 14: NumberInt(16), 15: NumberInt(15), 16: NumberInt(14)
		},
		date:  getTimeExpand(localDate, 3600*i)
	});
}


// ScrewByMinute_1
// structer
// {
// 	_id: number,
//	total: the total of screw
// 	screw: the number of screw failure,
// 	draft: the number of draft failure,
// 	feeder: the number of feeder failure,
// 	fixture: the number of fixture failure,
// 	screwPosition: [[position of screw failure, the number of failure ], [position of screw failure, the number of failure], ...  ],
// 	date: DateTime
// }


for (var i = 0; i < totalMinute; i++) {
	var tt = (60 + (Math.floor(Math.random()*20) - 10)) * 16;
	var fl = (50 + (Math.floor(Math.random()*30) - 15));

	db[ScrewByMinute_1].insert({
		_id: NumberInt(i),
		total: NumberInt(tt),
		screw: NumberInt(fl),
		draft: {
			1: NumberInt(15 + (Math.floor(Math.random()*6) - 3)),
			2: NumberInt(15 + (Math.floor(Math.random()*6) - 3))
		},
		feeder: NumberInt(50 + (Math.floor(Math.random()*30) - 15)),
		fixture: NumberInt(2 + (Math.floor(Math.random()*4) - 2)),
		screwPosition: {
			1: NumberInt(1), 2: NumberInt(2), 3: NumberInt(3), 4: NumberInt(4),
			5: NumberInt(5), 6: NumberInt(6), 7: NumberInt(7), 8: NumberInt(8),
			9: NumberInt(9), 10: NumberInt(10), 11: NumberInt(11), 12: NumberInt(12),
			13: NumberInt(13), 14: NumberInt(14), 15: NumberInt(15), 16: NumberInt(16)
		},
		date: getTimeExpand(localDate, 900*i)
	});
}

// ScrewByDay_2
// structer
// {
// 	_id: number,
//	total: the total of screw
// 	screw: the number of screw failure,
// 	draft: the number of draft failure,
// 	feeder: the number of feeder failure,
// 	fixture: the number of fixture failure,
// 	screwPosition: [[position of screw, the number of failure], [position of screw, the number of failure]],
// 	date: DateTime
// }


for (var i = 0; i < totalDay; i++) {
	var tt = (2500 + (Math.floor(Math.random()*500) - 250)) * 16;
	var fl = (50 + (Math.floor(Math.random()*30) - 15));

	db[ScrewByDay_2].insert({
		_id: NumberInt(i),
		total: NumberInt(tt),
		screw: NumberInt(fl),
		draft: {
			1: NumberInt(15 + (Math.floor(Math.random()*6) - 3)),
			2: NumberInt(15 + (Math.floor(Math.random()*6) - 3))
		},
		feeder: NumberInt(30 + (Math.floor(Math.random()*20) - 10)),
		fixture: NumberInt(2 + (Math.floor(Math.random()*4) - 2)),
		screwPosition: {
			1: NumberInt(1), 2: NumberInt(2), 3: NumberInt(3), 4: NumberInt(4),
			5: NumberInt(5), 6: NumberInt(6), 7: NumberInt(7), 8: NumberInt(8),
			9: NumberInt(9), 10: NumberInt(10), 11: NumberInt(11), 12: NumberInt(12),
			13: NumberInt(13), 14: NumberInt(14), 15: NumberInt(15), 16: NumberInt(16)
		},
		date:  getTimeExpand(localDate, 86400*i)
	});
}

// ScrewByHour_2
// structer
// {
// 	_id: number,
//	total: the total of screw
// 	screw: the number of screw failure,
// 	draft: the number of draft failure,
// 	feeder: the number of feeder failure,
// 	fixture: the number of fixture failure,
// 	screwPosition: [[position of screw, the number of failure], [position of screw, the number of failure]],
// 	date: DateTime
// }


for (var i = 0; i < totalHour; i++) {
	var tt = (250 + (Math.floor(Math.random()*50) - 24)) * 16;
	var fl = (50 + (Math.floor(Math.random()*30) - 15));

	db[ScrewByHour_2].insert({
		_id: NumberInt(i),
		total: NumberInt(tt),
		screw: NumberInt(fl),
		draft: {
			1: NumberInt(15 + (Math.floor(Math.random()*6) - 3)),
			2: NumberInt(15 + (Math.floor(Math.random()*6) - 3))
		},
		feeder: NumberInt(30 + (Math.floor(Math.random()*20) - 10)),
		fixture: NumberInt(2 + (Math.floor(Math.random()*4) - 2)),
		screwPosition: {
			1: NumberInt(2), 2: NumberInt(1), 3: NumberInt(3), 4: NumberInt(4),
			5: NumberInt(5), 6: NumberInt(6), 7: NumberInt(8), 8: NumberInt(7),
			9: NumberInt(9), 10: NumberInt(10), 11: NumberInt(4), 12: NumberInt(12),
			13: NumberInt(13), 14: NumberInt(16), 15: NumberInt(15), 16: NumberInt(14)
		},
		date:  getTimeExpand(localDate, 3600*i)
	});
}


// ScrewByMinute_2
// structer
// {
// 	_id: number,
//	total: the total of screw
// 	screw: the number of screw failure,
// 	draft: the number of draft failure,
// 	feeder: the number of feeder failure,
// 	fixture: the number of fixture failure,
// 	screwPosition: [[position of screw failure, the number of failure ], [position of screw failure, the number of failure], ...  ],
// 	date: DateTime
// }


for (var i = 0; i < totalMinute; i++) {
	var tt = (60 + (Math.floor(Math.random()*20) - 10)) * 16;
	var fl = (50 + (Math.floor(Math.random()*30) - 15));

	db[ScrewByMinute_2].insert({
		_id: NumberInt(i),
		total: NumberInt(tt),
		screw: NumberInt(fl),
		draft: {
			1: NumberInt(15 + (Math.floor(Math.random()*6) - 3)),
			2: NumberInt(15 + (Math.floor(Math.random()*6) - 3))
		},
		feeder: NumberInt(50 + (Math.floor(Math.random()*30) - 15)),
		fixture: NumberInt(2 + (Math.floor(Math.random()*4) - 2)),
		screwPosition: {
			1: NumberInt(1), 2: NumberInt(2), 3: NumberInt(3), 4: NumberInt(4),
			5: NumberInt(5), 6: NumberInt(6), 7: NumberInt(7), 8: NumberInt(8),
			9: NumberInt(9), 10: NumberInt(10), 11: NumberInt(11), 12: NumberInt(12),
			13: NumberInt(13), 14: NumberInt(14), 15: NumberInt(15), 16: NumberInt(16)
		},
		date: getTimeExpand(localDate, 900*i)
	});
}
