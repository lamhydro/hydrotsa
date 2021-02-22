#BASE = /home/luis/Documents/projects/tsana
CC		= gcc
CFLAGS = -O -Wall -pedantic -Wstrict-prototypes -g #-pg
#CFLAGS = -O -Wall
TARGETS := bin/tspre
#TARGETS := freqa/bin/freqa lireg/bin/lireg specta/bin/specta tspre/bin/tspre
INCLS = -I./inc
LIBS = -lm 
LFLAGS = 

#SRC = $(wildcard src/*.c)
SRC1 = src/utils.c src/stats.c src/aggts.c src/main_tspre.c src/tspre.c 

OBJS1 = $(SRC1:.c=.o) 

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
bin/tspre: $(OBJS1)
	$(CC) $(CFLAGS) $(INCLS) -o bin/tspre $(OBJS1) $(LFLAGS) $(LIBS)

#.o.c:
%.o: %.c
	$(CC) $(CFLAGS) $(INCLS) -o $@ -c $<
	
.PHONY: clean
clean:
	rm -rf $(TARGETS) $(OBJS1) a.out

.PHONY: test
test:
	bin/tspre test/ ts.bin

