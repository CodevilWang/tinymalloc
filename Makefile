CXX=gcc
SRC='tiny_malloc.c'
FLAGS=-O0 -shared -fPIC
TARGET=malloc.so
all:
	${CXX} ${SRC} ${FLAGS} -o ${TARGET}
clean:
	@rm ${TARGET}
