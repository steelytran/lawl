WATCOM=/usr/local/src/open-watcom-v2/rel
CC = $WATCOM/armo64/wcc386
LD = $WATCOM/armo64/wlink
INCLUDE=$WATCOM/h

CFLAGS = -3 -fpi87 -fp3 -os -d0 -mf
LFLAGS = sys dos32a

EXEC = ../a.exe
SRC = `{ls *.c}
OBJ = ${SRC:%.c=%.obj}

%.obj: %.c
	$CC $CFLAGS -fo=$stem.obj $stem.c

$EXEC: $OBJ
	PATH=$WATCOM/armo64:$WATCOM/binw:$PATH $LD name $EXEC $LFLAGS file { $OBJ }

clean:V:
	rm -f *.obj $EXEC *.err