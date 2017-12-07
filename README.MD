## Author Nguyen Hai Duong
###	Date Dec 6, 2017
###	Version 1.0

The main program communicate with multi PLC by USB(raspberry) <-> UART(PLC). PLC will automaticly send error codes and total of screwing to the program. The program after received codes/total will save that codes/total into mongodb database.


## Feature:

-	Database 
	Datbase name is "ScrewMachine", collection name are: "ScrewByDay_*", "ScrewByHour_*", "ScrewByMinute_*" (* is mark for PLC, ex: 1,2,..). All documents in collections will be inserted by function: insertInOrderWithDateTime(). The "ScrewByMinute_*" will automaticly create a new one after each 15 minutes, "ScrewByHour_*" auto create in hour, "ScrewByDay_*" auto create at new day.	
The database havent fixed the size of collection. Collections will be overload in the future.

## Todo:

1.	The program has not known what PLC is connecting to it. Need function to verify PLC.
2.	Writle log by month. Write config mode by file.
3.	New feature: Class read "air vacumm pressure". Commnunicate with Station by tcp/ip.
4.	Create new c++ library communicate with mongodb. Should did not create new document in order (dont use insertInOrderWithDateTime()).	