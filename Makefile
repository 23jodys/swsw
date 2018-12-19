SRCS := score_matrix.c alignment.c sw.c
OBJS := $(SRCS:.c=.o)
GCOV := llvm-cov gcov
CC   := clang

CFLAGS += $(if $(COVERAGE), -fprofile-arcs -ftest-coverage )

LDLIBS += $(if $(or $(COVERAGE),$(DEBUG)), -g )
LDLIBS += $(if $(COVERAGE), --coverage )

CFLAGS += -Werror

all: test_score_matrix test_alignment test_sw

test_score_matrix: LDLIBS += -lcmocka
test_score_matrix: score_matrix.o test_score_matrix.o alignment.o sds/sds.o

test_alignment: LDLIBS += -lcmocka 
test_alignment: alignment.o test_alignment.o sds/sds.o

test_sw: LDLIBS += -lcmocka
test_sw: sw.o alignment.o score_matrix.o test_sw.o sds/sds.o 

score_matrix.o: score_matrix.h
alignment.o: alignment.h
sw.o: swsw.h

valgrind_%: %
	valgrind --leak-check=full --suppressions=valgrind_suppressions.supp --error-exitcode=1 ./$*

coverage: test
	$(GCOV) $(SRCS)

.PHONY: test
test: test_alignment test_score_matrix test_sw
	./test_alignment
	./test_score_matrix
	./test_sw

TAGS: $(SRCS) alignment.h score_matrix.h swsw.h test_*.[ch]
	etags $^

.PHONY: clean
clean:
	rm -rf *.o test_score_matrix test_alignment test_sw sds/*.o *.gcda *.gcno
