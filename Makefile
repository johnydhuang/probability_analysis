CC = gcc
CFLAGS += 

INC = 
INC += -I/usr/local/include/
INC += -I.

LDFLAG = -lm

OBJ += prob_analysis.o
BIN = prob_analysis

all:
	${CC} ${CFLAGS} prob_analysis.c ${INC} -o ${BIN} 
	
clean:
	rm -f *.o ${BIN}
