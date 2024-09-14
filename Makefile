#dnsthing

# dependencies
LIBMONGOC 	= /usr/include/libmongoc-1.0
LIBBSON		= /usr/include/libbson-1.0
LIBCURL		= /usr/include/curl

# version of dnsthing
VER             = 0.0.1

# compiler type and flags
CC		= gcc
INC		= -lmongoc-1.0 -lbson-1.0 -lcurl -I$(LIBCURL) -I$(LIBMONGOC) -I$(LIBBSON)
CFLAGS		= $(INC) -std=c99 -pedantic -Wall -DVER=\"${VER}\"




# targets
TARGET = dnsthing
OBJ = checks.o database.o dnsthing.o util.o

all: $(TARGET)

dnsthing: $(OBJ)
	${CC} -o $@ ${OBJ} ${CFLAGS} $(INC) 

clean:
	rm -f $(TARGET) ${OBJ}
