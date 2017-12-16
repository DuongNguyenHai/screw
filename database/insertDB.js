var ScrewByMinute_1 = "ScrewByMinute_1";
var ScrewByHour_1 = "ScrewByHour_1";
var ScrewByDay_1 = "ScrewByDay_1";

var ScrewByMinute_2 = "ScrewByMinute_2";
var ScrewByHour_2 = "ScrewByHour_2";
var ScrewByDay_2 = "ScrewByDay_2";

db = connect("localhost:27017");
db = db.getSiblingDB("ScrewMachine");

var GMT = 0;
var day = 0;
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

var totalDay = 0;
var totalHour = totalDay*24 + 2;
var totalMinute = totalDay*24*4 + 0;

db[ScrewByDay_2].update({_id:"id"},{$set:{"total":25}});

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

for (var i = 24; i < 24 + totalHour; i++) {
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
