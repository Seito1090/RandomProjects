CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=-lcunit -lpthread
INCLUDE_HEADERS_DIRECTORY=-Iinclude
SHELL := /bin/bash

help:
	@echo "+------------------------------------------------------------------------------------+"
	@echo "|  make sp      : compile the program                                                |"
	@echo "|  make mygraph : compile the program and run it with your own graph as a ntf file   |"
	@echo "|  make myfile  : compile the program and run it with your own graph as a bin file   |"
	@echo "|  make tests   : compile and run the tests                                          |"
	@echo "|  make clean   : remove all the object files and the executable that were created   |"
	@echo "|  make help    : show this help message                                             |"
	@echo "+------------------------------------------------------------------------------------+"

# compile the program with gcc
sp: src/graph.o sp.c 
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o main $^    

#%.o: %.c                  # if for example you want to compute example.c this will create an object file called example.o in the same directory as example.c. Don't forget to clean it in your "make clean"
#	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $<

run: 
	@make -s sp 
	@./main tests/graph_bin/default.bin

time:
	@make -s sp
	@echo "Running the program with the default graph"
	@time ./main tests/graph_bin/default.bin

clean:
	@rm -f src/*.o
	@rm -f main
	@rm -f graphMT
	@rm -f tests/*.o
	@rm -f tests/cunit_tests
	@rm -f my*.bin
	@rm -f tests/graph_bin/p_*
	@echo "Cleared"

tests: src/graph.o tests/cunittests.c
	   $(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o tests/cunit_tests $^ $(LIBS)

# run them with valgrind to check for memory leaks and see output
#	   valgrind --leak-check=full ./tests/cunit_tests
	@echo "--------------------------------------------------------------------------------------------------------------" 
# now run the main program with valgrind to show that there are no leaks, input file default
	@make -s sp
	valgrind --leak-check=full ./main tests/graph_bin/default.bin

mygraph:
	@make -s sp
	@echo "Enter the name of your ntf file (without extension): "
	@read FileName; echo "Creating the bin file"; \
	python3 tests/necessary_python/create_graph.py -t tests/ntf_files/$$FileName.ntf -o tests/graph_bin/p_$$FileName.bin; \
	echo "Done"; \
	echo "Running the program with your graph"; \
	./main tests/graph_bin/p_$$FileName.bin -f myGraphOut.bin; \
	echo "Done"; 

myfile:
	@make -s sp
	@echo "Enter the name of your bin file (without extension): "
	@read FileName;\
	echo "Running the program with the graph : $$FileName.bin and saving it to myFileOut.bin"; \
	./main tests/graph_bin/$$FileName.bin -f myFileOut.bin; \
	echo "Done";

mt:	src/graph.o src/graphMT.c 
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o graphMT $^ 

# a .PHONY target forces make to execute the command even if the target already exists
.PHONY: clean tests time

