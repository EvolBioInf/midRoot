CC=gcc
export VERSION = $(shell bash ../scripts/version.sh) # The single, trailing blank is essential
export DATE    = $(shell bash ../scripts/date.sh) #    The single, trailing blank is essential
CFLAGS         = -DVERSION=\"$(VERSION)\" -DDATE=\"$(DATE)\" -O3 -Wall -Wshadow -pedantic -std=gnu99 -g -I../common/ -L../common/ #-pg

SRCFILES  = midRoot.c interface.c 
OBJFILES  = midRoot.o interface.o 
LIBS      = -lcommon -lm
EXECFILE  = midRoot

.PHONY : all
all : $(EXECFILE)
$(EXECFILE) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $(EXECFILE) $(OBJFILES) $(LIBS)
interface.o: interface.h
midRoot.c: midRoot.org
	bash ../scripts/org2nw midRoot.org | notangle -RmidRoot.c > midRoot.c

clean:
	rm -f *.o *~ midRoot.c
test:
	bash ../scripts/midRoot.sh 
