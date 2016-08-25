CC = gcc
OBJS = SPConfig_Unit_Test.o SPConfig.o SPLogger.o
<<<<<<< HEAD
EXEC = SPConfig_Unit_Test.c
=======
EXEC = SPConfig_Unit_Test
>>>>>>> refs/remotes/origin/master
TESTS_DIR = ./testers/Config
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
<<<<<<< HEAD
SPConfig_Unit_Test.o: $(TESTS_DIR)/SPConfig_Unit_Test.c $(TESTS_DIR)/unit_test_util.h SPLogger.h SPConfig.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
SPConfig.o: SPLogger.h SPConfig.c
	$(CC) $(COMP_FLAG) -c $*.c
SPLogger.o: SPLogger.h SPConfig.c
=======
SPConfig_Unit_Test.o: $(TESTS_DIR)/SPConfig_Unit_Tes.c $(TESTS_DIR)/unit_test_util.h SPLogger.h SPConfig.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
SPConfig.o: SPConfig.c SPLogger.h
	$(CC) $(COMP_FLAG) -c $*.c
SPLogger.o: SPLogger.c SPLogger.h
>>>>>>> refs/remotes/origin/master
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
