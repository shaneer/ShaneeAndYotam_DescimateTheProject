CC = gcc
OBJS = SPKDArray_unit_test.o SPKDArray.o SPPoint.o
EXEC = sp_kdarray_unit_test
TESTS_DIR = ./testers
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
SPKDArray_unit_test.o: $(TESTS_DIR)/SPKDArray_unit_test.c $(TESTS_DIR)/unit_test_util.h SPPoint.h SPKDArray.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
SPKDArray.o: SPKDArray.c SPPoint.h
	$(CC) $(COMP_FLAG) -c $*.c
SPPoint.o: SPPoint.c SPPoint.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
