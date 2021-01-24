#BASE = /home/luis/Documents/projects/tsana
CC		= gcc
CFLAGS = -O -Wall -pedantic -Wstrict-prototypes -g #-pg
#CFLAGS = -O -Wall
TARGETS := tspre/bin/tspre
#TARGETS := freqa/bin/freqa lireg/bin/lireg specta/bin/specta tspre/bin/tspre
INCLS = -I./utils/inc -I./freqa/inc -I./lireg/inc -I./specta/inc -I./tspre/inc
LIBS = -lm 
LFLAGS = 

SRC0 = $(wildcard utils/src/*.c)
SRC1 = $(wildcard freqa/src/*.c) $(SRC0)
SRC2 = $(wildcard lireg/src/*.c) $(SRC0)
SRC3 = $(wildcard specta/src/*.c) $(SRC0)
SRC4 = $(wildcard tspre/src/*.c) $(SRC0)

#SRC = src/aggts.c src/freqa.c src/lireg.c src/stats.c src/tspre.c src/utils.c src/gev.c
#SRC1 = src/frequana.c $(SRC) 
#SRC2 = src/spectana.c $(SRC) 
#SRC3 = src/trendana.c $(SRC) 
#SRC4 = src/tsaggreg.c $(SRC) 
#SRC5 = src/tscleani.c $(SRC) 

OBJS1 = $(SRC1:.c=.o) 
OBJS2 = $(SRC2:.c=.o) 
OBJS3 = $(SRC3:.c=.o) 
OBJS4 = $(SRC4:.c=.o) 

#$(phony all): $(TARGETS)
#.PHONY: all $(TARGETS)
all: $(TARGETS)
#freqa/bin/freqa: $(OBJS1)
#	$(CC) $(CFLAGS) $(INCLS) -o freqa/bin/freqa $(OBJS1) $(LFLAGS) $(LIBS)

#lireg/bin/lireg: $(OBJS2)
#	$(CC) $(CFLAGS) $(INCLS) -o lireg/bin/lireg $(OBJS2) $(LFLAGS) $(LIBS)

#specta/bin/specta: $(OBJS3)
#	$(CC) $(CFLAGS) $(INCLS) -o specta/bin/specta $(OBJS3) $(LFLAGS) $(LIBS)
#
tspre/bin/tspre: $(OBJS4)
	$(CC) $(CFLAGS) $(INCLS) -o tspre/bin/tspre $(OBJS4) $(LFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLS) -o $@ -c $<
	
.PHONY: clean
clean:
	rm -rf $(TARGETS) $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) a.out

.PHONY: test
test:
	tspre/bin/tspre tspre/test/ ts.bin

#tspre/bin/tspre /home/luis/hub/canNatFlowRegi/data/ 08NG044_DLY_FLOWS.bin
