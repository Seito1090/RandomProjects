CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=-lcunit -lpthread
INCLUDE_HEADERS_DIRECTORY=-Iinclude
SHELL := /bin/bash

help:
	@echo "+------------------------------------------------------------------------------------+"
	@echo "|  make sp      : compile the program                                                |"
	@echo "|  make run     : compile and run the program with default behaviour                 |"
	@echo "|  make time    : compile and run the program with default behaviour and time it     |"
	@echo "|  make myfile  : compile the program and run it with your own graph as a bin file   |"
	@echo "|  make mygraph : compile the program and run it with your own graph as a ntf file   |"
	@echo "|  make tests   : compile and run the tests                                          |"
	@echo "|  make clean   : remove all the object files and the executable that were created   |"
	@echo "|  make help    : show this help message                                             |"
	@echo "+------------------------------------------------------------------------------------+"

# compile the program with gcc
sp: src/graph.c src/mtgraph.c sp.c 
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o main $^ $(LIBS)    

%.o: %.c                  # if for example you want to compute example.c this will create an object file called example.o in the same directory as example.c. Don't forget to clean it in your "make clean"
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $<

run: 
	@make -s sp 
	@./main tests/graph_bin/default.bin

time:
	@make -s sp
	@echo "Running the program with the default graph"
	@echo "1 thread"
	@time ./main tests/graph_bin/default.bin -f time.bin -n 1 
	@echo "-----------------------------------------------------------------------------------" 
	@echo "2 threads"
	@time ./main tests/graph_bin/default.bin -f time.bin -n 2 
	@echo "-----------------------------------------------------------------------------------" 
	@echo "4 threads"
	@time ./main tests/graph_bin/default.bin -f time.bin -n 4 
	@echo "-----------------------------------------------------------------------------------" 
	@echo "10 threads"
	@time ./main tests/graph_bin/default.bin -f time.bin -n 10 
	@echo "-----------------------------------------------------------------------------------" 
	@echo "100 threads"
	@time ./main tests/graph_bin/default.bin -f time.bin -n 100 
	@echo "-----------------------------------------------------------------------------------" 
	@echo "200 threads"
	@time ./main tests/graph_bin/default.bin -f time.bin -n 200 


clean:
	@rm -f src/*.o
	@rm -f main
	@rm -f mtgraph
	@rm -f tests/*.o
	@rm -f tests/cunit_tests
	@rm -f my*.bin
	@rm -f tests/graph_bin/p_*
	@rm -f *.bin
	@echo "Cleared"

tests: src/graph.c tests/cunittests.c
	   $(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o tests/cunit_tests $^ $(LIBS)

# run them with valgrind to check for memory leaks and see output
#	   valgrind --leak-check=full ./tests/cunit_tests
	@./tests/cunit_tests
	@echo "--------------------------------------------------------------------------------------------------------------" 
# now run the main program with valgrind to show that there are no leaks, input file default
	@make -s sp
	valgrind --tool=helgrind -s ./main tests/graph_bin/default.bin -f test.bin
	@echo "--------------------------------------------------------------------------------------------------------------" 
	valgrind --leak-check=full --show-leak-kinds=all -s ./main tests/graph_bin/default.bin -f test.bin

myfile:
	@make -s sp
	@echo "Enter the name of your bin file (or path if not in this directory): "
	@read FileName;\
	echo "Running the program with the graph : $$FileName.bin and saving it to myFileOut.bin"; \
	./main $$FileName -f outputs/myFileOut.bin; \
	echo "Done";

mygraph:
	@make -s sp
	@echo "Enter the name of your ntf file (without extension): "
	@read FileName; echo "Creating the bin file"; \
	python3 tests/necessary_python/create_graph.py -t ntf_files/$$FileName.ntf -o tests/graph_bin/p_$$FileName.bin; \
	echo "Done"; \
	echo "Running the program with your graph"; \
	./main tests/graph_bin/p_$$FileName.bin -f outputs/myGraphOut.bin; \
	echo "Done"; 


mt:	src/graph.c src/mtgraph.c 
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o mtgraph $^ 
	./mtgraph

# a .PHONY target forces make to execute the command even if the target already exists
.PHONY: clean tests time

