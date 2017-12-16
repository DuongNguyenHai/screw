## Author Nguyen Hai Duong
#####	Date Dec 6, 2017
#####	Version 1.0

The main program communicate with multi PLC by USB(raspberry) <-> UART(PLC). PLC will automacticlly send error codes and total of screwing to the program. The program after received codes/total will save that codes/total into mongodb database.

## Feature

-	Recording failure of machine, the number of total screw times.
-	Recording pressure of vacuum (a part of self-test program).
-	Save data at local machine and send data to server by tcp/ip network.

## Specification

-	Database
	Database name is "ScrewMachine", collection name are: "ScrewByDay_*", "ScrewByHour_*", "ScrewByMinute_*" (* is mark for PLC, ex: 1,2,..). All documents in collections will be inserted by function: insertInOrderWithDateTime(). The "ScrewByMinute_*" will automatically create a new one after each 15 minutes, "ScrewByHour_*" auto create in hour, "ScrewByDay_*" auto create at new day. The database haven't fixed the size of collection. Collections will be overload in the future.

## Todo

1.	DONE - The program has not known what PLC is connecting to it. Need function to verify PLC.
2.	DONE - Write log by month. Write config mode by file. DONE
3.	WORKING - New feature: Class read "air vacuum pressure". Communicate with Station by tcp/ip.
4.	Create new c++ library communicate with mongodb. Should did not create new document in order (don't use insertInOrderWithDateTime()).
