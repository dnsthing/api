#dnsthing

CC = cc

TARGET = dnsthing
OBJ = api.o dnsthing.o

all: $(TARGET)

dnsthing: $(OBJ)
	${CC} -o $@ ${OBJ} ${CFLAGS} 

clean:
	rm -f $(TARGET) ${OBJ}
