CXX=gcc
SRC='tiny_malloc.c'
TEST_SRC='test_main.c'
FLAGS=-g -O0 -shared -fPIC
TARGET=malloc.so
TEST_TARGET=test
all:
	${CXX} ${SRC} ${FLAGS} -o ${TARGET}
	${CXX} ${TEST_SRC} -g -O0 -o ${TEST_TARGET} 
clean:
	@rm ${TARGET}
	@rm ${TEST_TARGET}
