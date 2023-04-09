CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=-lcunit -lpthread
INCLUDE_HEADERS_DIRECTORY=-Iinclude

# compile the program with gcc
sp: src/graph.o sp.c 
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o main $^    

#%.o: %.c                  # if for example you want to compute example.c this will create an object file called example.o in the same directory as example.c. Don't forget to clean it in your "make clean"
#	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $<

clean:
	rm -f src/*.o
	rm -f main
	rm -f tests/*.o
	rm -f tests/cunit_tests
	# ...

tests: src/graph.o tests/cunittests.c
	   $(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o tests/cunit_tests $^ $(LIBS)
# run them to show they at least compile lol
	   ./tests/cunit_tests
# run them with valgrind to check for memory leaks
	   valgrind --leak-check=full ./tests/cunit_tests
	   
# now run the main program with valgrind to show that there are no leaks, input file default
	   make sp
	   valgrind --leak-check=full ./main tests/graph_bin/default.bin

# a .PHONY target forces make to execute the command even if the target already exists
.PHONY: clean tests
