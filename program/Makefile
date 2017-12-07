CC = gcc
CXX = g++

ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

DIRLIB = $(ROOT_DIR)/lib
DIRSYS = $(ROOT_DIR)/system
DIRLIB_BASE = $(DIRLIB)/base

CFLAGS += -std=c++11 -Wall -g
LDFLAGS += -lpthread
INCLUDES += -I$(DIRLIB) -I$(DIRLIB_BASE) -I$(DIRSYS)
LIBS += `pkg-config --cflags --libs libmongoc-1.0`

SOURCES = $(DIRLIB_BASE)/bone-log.cc $(DIRLIB_BASE)/bone-database.cc $(DIRLIB)/database.cc $(DIRSYS)/screw-machine.cc $(DIRLIB)/usb-port.cc main.cc

OBJECTS = $(addsuffix .o, $(basename $(SOURCES)))
OBJECTS_d = $(addsuffix .d, $(basename $(SOURCES)))

EXE = screwer

all: $(EXE)

#note : Write command in order : $(CXX) $(CFLAGS) $(OBJECTS) $(LDFLAGS) $(INCLUDES) $(LIBS) -o $(EXE)
$(EXE): $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) $(LDFLAGS) $(INCLUDES) $(LIBS) -o $(EXE)

-include $(OBJECTS:.o=.d)

%.o: %.cc
	$(CXX) -c $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(LIBS) $*.cc -o $*.o

	$(CXX) -MM $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(LIBS) $*.cc > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(LIBS) $*.cpp -o $*.o

	$(CXX) -MM $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(LIBS) $*.cpp > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

clean:
	$(RM) $(OBJECTS) $(OBJECTS_d) $(EXE)
