CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=-lcunit -lpthread
INCLUDE_HEADERS_DIRECTORY=-Iinclude
INCLUDE_CUNIT_DIRECTORY=-I$(HOME)/CUnit-2.1-3/include

# compile the program with gcc
sp: src/graph.o sp.c 
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o main $^    

#%.o: %.c                  # if for example you want to compute example.c this will create an object file called example.o in the same directory as example.c. Don't forget to clean it in your "make clean"
#	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $<

clean:
	rm -f src/*.o
	rm -f main
	rm -f tests/CUnit/*.o
	rm -f tests/graph_bin/graph*
	rm -f tests/result_bin/result*
	# ...

tests: 
#	tests/CUnit/cunit.c
#	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(INCLUDE_CUNIT_DIRECTORY) $(CFLAGS) -o tests $^ $(LIBS)

#	create the graph files for inputs of tests 
	python tests/necessary_python/create_graph.py --ntf tests/ntf_files/negative_cost.ntf --output tests/graph_bin/graph_neg.bin
	python tests/necessary_python/create_graph.py --ntf tests/ntf_files/zeros_everywhere.ntf --output tests/graph_bin/graph_0.bin

#   run the python code to compute the expected outputs of tests	
	python tests/necessary_python/sp.py tests/graph_bin/default.bin -f tests/result_bin/result_def.bin
	python tests/necessary_python/sp.py tests/graph_bin/graph_neg.bin -f tests/result_bin/result_neg.bin

# 	Do not forget the -lcunit flag
	cd tests/CUnit
#	./tests
	cd ../..
	
# a .PHONY target forces make to execute the command even if the target already exists
.PHONY: clean tests
