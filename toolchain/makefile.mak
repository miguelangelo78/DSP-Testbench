CC=gcc
CFLAGS=-std=c99 -Iinclude

RM=rm

SRC=src
TEST=test
BIN=bin
OBJ=obj

EXE=dsp

GNUPLOT=lib/gnuplot/bin/gnuplot
GNUPLOT_DATA_PATH=data/gnuplot
GNUPLOT_CONFIG=$(GNUPLOT_DATA_PATH)/default.gnuplot
SAMPLEFILE=$(GNUPLOT_DATA_PATH)/default.csv
LOCKFILE=$(GNUPLOT_DATA_PATH)/lock

TEST_SOURCES=$(wildcard test/*.c)
TEST_OBJECTS=$(wildcard obj/*.o)

all: update
ifeq ("$(wildcard $(LOCKFILE))","")
	$(MAKE) -f $(subst \,/,$(lastword $(MAKEFILE_LIST))) run || $(MAKE) -f $(subst \,/,$(lastword $(MAKEFILE_LIST))) on_run_error &
endif
	@printf "\nDone"

clean:
	$(RM) -f $(BIN)/*.o
	$(RM) -f $(OBJ)/*.o
	$(RM) -f $(LOCKFILE)

update:
	$(BIN)/$(EXE) $(SAMPLEFILE)

run:
	@echo > $(LOCKFILE)
	@printf "\n>> Press 'ESC' to quit gnuplot\n"
	@$(GNUPLOT) $(GNUPLOT_CONFIG)
	@$(RM) -f $(LOCKFILE)

on_run_error:
	@$(RM) -f $(LOCKFILE)

test_%:
	$(eval testname=$(subst .c,,$(subst test_,,$@)))
	$(CC) -c $(TEST)/$(testname).c -o $(OBJ)/$(testname).o $(CFLAGS)
	$(CC) -c $(SRC)/dsp.c -o $(OBJ)/$(EXE).o $(CFLAGS)
	$(CC) -o $(BIN)/$(EXE) $(CFLAGS) $(OBJ)/$(testname).o $(OBJ)/$(EXE).o