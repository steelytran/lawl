WATCOM=../../../open-watcom-v2/rel
CC = wcc
LINKER = wlink

CFLAGS = -zq -ml -fpc
LFLAGS = sys dos libpath $(WATCOM)\lib286\dos

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
