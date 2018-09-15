test_score_matrix: LDLIBS += -lcmocka
test_score_matrix: score_matrix.o test_score_matrix.o alignment.o

test_alignment: LDLIBS += -lcmocka 
test_alignment: CFLAGS += -g
test_alignment: alignment.o test_alignment.o

TARGET := libswsw.dylib

.PHONY: test
test: test_alignment test_score_matrix
	./test_alignment
	./test_score_matrix

LIBRARY := libswsw.dylib

$(LIBRARY): alignment.o score_matrix.o swsw.h
	gcc -dynamiclib -o $@ $^
