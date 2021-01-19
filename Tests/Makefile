CC=g++

GTESTINSTALLDIR=C:/tdd/googletest-master
GTESTINCLUDE_PATH=$(GTESTINSTALLDIR)/googletest/include
GTESTLINK_PATH=$(GTESTINSTALLDIR)/build/lib
GMOCKINCLUDE_PATH=$(GTESTINSTALLDIR)/googlemock/include

GTESTLIBS=-lgtest -lgtest_main -lgmock -lgmock_main

TestFileName=STimertestRunner

EXECEXTENSION=.exe

$(TestFileName)$(EXECEXTENSION) : $(TestFileName).cpp
	$(CC) $(TestFileName).cpp -DGTEST_HAS_PTHREAD=1 -I $(GTESTINCLUDE_PATH) -I $(GMOCKINCLUDE_PATH) -L $(GTESTLINK_PATH) $(GTESTLIBS) -o $(TestFileName)
