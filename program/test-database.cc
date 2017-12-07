#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "bone-log.h"
#include "database.h"

using namespace BONE;
#define DATABASE_NAME "ScrewMachine"


int main(int argc, char const *argv[])
{
	PRINT_FILE = false;
    DEBUG_LEVEL = 3;

    Database dtbase;
    dtbase.begin(DATABASE_NAME);

    dtbase.increaseElement(LogRealTime, TOTAL, 16);
    dtbase.increaseElement(LogRealTime, DRAFT, 1);
    dtbase.increaseElement(LogRealTime, FEEDER, 1);
    dtbase.increaseElement(LogRealTime, FIXTURE, 1);
	
	dtbase.increaseScrewPosition(LogRealTime, 1, 1);
    dtbase.increaseScrewPosition(LogRealTime, 2, 1);
    //dtbase.insertNewProductDoct();
    // dtbase.insertNewLogRealTimeDoct();
    //LOG << dtbase.aaa();
	return 0;
}

