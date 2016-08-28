CC = gcc
OBJS = SPConfig_Unit_Test.o SPConfig.o
EXEC = SPConfig_Unit_Test
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
SPConfig_Unit_Test.o: SPConfig_Unit_Test.c unit_test_util.h SPConfig.h
	$(CC) $(COMP_FLAG) -c $*.c
SPConfig.o: SPConfig.h SPConfig.c
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
