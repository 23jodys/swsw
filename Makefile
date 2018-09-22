SRCS := score_matrix.c alignment.c
OBJS := $(SRCS:.c=.o)

test_score_matrix: LDLIBS += -lcmocka
test_score_matrix: CFLAGS += -g
test_score_matrix: score_matrix.o test_score_matrix.o alignment.o

test_alignment: LDLIBS += -lcmocka 
test_alignment: CFLAGS += -g
test_alignment: alignment.o test_alignment.o

valgrind_%: %
	valgrind --error-exitcode=1 ./$*

.PHONY: test
test: test_alignment test_score_matrix
	./test_alignment
	./test_score_matrix

libswsw.dylib: alignment.o score_matrix.o swsw.h
	gcc -dynamiclib -o $@ $^

TAGS: $(SRCS) alignment.h score_matrix.h
	etags $^
