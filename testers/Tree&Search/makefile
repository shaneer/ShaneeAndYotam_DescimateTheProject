CC = gcc
OBJS = Search_Unit_Test.o SPKDTreeNNSearch.o SPKDArray.o SPPoint.o SPConfig.o SPBPQueue.o
EXEC = Search_Unit_Test
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
SPConfig_Unit_Test.o: Search_Unit_Test.c unit_test_util.h SPKDTreeNNSearch.h SPKDArray.h SPPoint.h SPConfig.h SPBPQueue.h SPList.h SPListElement.h
	$(CC) $(COMP_FLAG) -c $*.c
SPKDTreeNNSearch.o: SPKDTreeNNSearch.h SPKDTreeNNSearch.c
	$(CC) $(COMP_FLAG) -c $*.c
SPKDArray.o: SPKDArray.h SPKDArray.c
	$(CC) $(COMP_FLAG) -c $*.c
SPPoint.o: SPPoint.h SPPoint.c
	$(CC) $(COMP_FLAG) -c $*.c
SPBPQueue.o: SPBPQueue.h SPBPQueue.c
	$(CC) $(COMP_FLAG) -c $*.c
SPList.o: SPList.h SPList.c
	$(CC) $(COMP_FLAG) -c $*.c
SPListElement.o: SPListElement.h SPListElement.c
	$(CC) $(COMP_FLAG) -c $*.c
SPConfig.o: SPConfig.h SPConfig.c
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
