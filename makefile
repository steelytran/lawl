WATCOM=../../../open-watcom-v2/rel
CC = wcc386
LINKER = wlink

CFLAGS = -zq -mf -3r -fpc
LFLAGS = sys dos32a op stub=$(WATCOM)/binw/dos32a.exe libpath $(WATCOM)\lib386\dos

MAIN_SRC = main.c
EXEC = ../a.exe

ODIR = obj

SOURCES = $(filter-out $(MAIN_SRC), $(wildcard *.c))

#OBJ = $(SOURCES:.c=.obj) $(MAIN_SRC:.c=.obj)
OBJ = $(patsubst %.c, $(ODIR)/%.obj, $(SOURCES)) $(patsubst %.c, $(ODIR)/%.obj, $(MAIN_SRC))

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJ)
	$(LINKER) name $@ $(LFLAGS) file { $(OBJ) }

$(ODIR)/%.obj: %.c
	$(CC) $(CFLAGS) -fo=$@ $<

clean:
	-@rm -f $(ODIR)/*.obj
	-@rm -f $(EXEC)
