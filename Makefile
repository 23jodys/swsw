SRCS := score_matrix.c alignment.c sw.c
OBJS := $(SRCS:.c=.o)
CC   := clang-7

test_score_matrix: LDLIBS += -lcmocka
test_score_matrix: CFLAGS += -g 
test_score_matrix: score_matrix.o test_score_matrix.o alignment.o sds/sds.o

test_alignment: LDLIBS += -lcmocka 
test_alignment: CFLAGS += -g 
test_alignment: alignment.o test_alignment.o sds/sds.o

test_sw: LDLIBS += -lcmocka
test_sw: CFLAGS += -g  -DDEBUG=1
test_sw: sw.o alignment.o score_matrix.o test_sw.o sds/sds.o 


valgrind_%: %
	valgrind --leak-check=full --suppressions=valgrind_suppressions.supp --error-exitcode=1 ./$*

.PHONY: test
test: test_alignment test_score_matrix test_sw
	./test_alignment
	./test_score_matrix
	./test_sw

libswsw.dylib: alignment.o score_matrix.o swsw.h
	gcc -dynamiclib -o $@ $^

TAGS: $(SRCS) alignment.h score_matrix.h swsw.h
	etags $^

.PHONY: clean
clean:
	rm -rf *.o test_score_matrix test_alignment test_sw sds/*.o
