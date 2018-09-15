test_score_matrix: LDLIBS += -lcmocka
test_score_matrix: score_matrix.o test_score_matrix.o

test_alignment: LDLIBS += -lcmocka 
test_alignment: CFLAGS += -g
test_alignment: alignment.o test_alignment.o

.PHONY: test
test: test_alignment test_score_matrix
	./test_alignment
	./test_score_matrix
