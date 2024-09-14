#dnsthing

# version of dnsthing
VER             = 0.0.1

# compiler type and flags
CC              = gcc
CFLAGS          = -std=c99 -pedantic -Wall -DVER=\"${VER}\"

# targets
TARGET = dnsthing
OBJ = dnsthing.o

all: $(TARGET)

dnsthing: $(OBJ)
	${CC} -o $@ ${OBJ} ${CFLAGS} 

clean:
	rm -f $(TARGET) ${OBJ}
