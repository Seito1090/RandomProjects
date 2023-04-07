CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=-lcunit -lpthread
INCLUDE_HEADERS_DIRECTORY=-Iinclude

sp: src/graph.o sp.c     # add your other object files needed to compile your program here. !! The ordering is important !! if file_a.o depends on file_b.o, file_a.o must be placed BEFORE file_b.o in the list !
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o main $^ $(LIBS)    
#this will run the following command: gcc -Wall -Werror -g -o sp sc.c other_object_filespresent_above.o ... -lcunit -lpthread

%.o: %.c                  # if for example you want to compute example.c this will create an object file called example.o in the same directory as example.c. Don't forget to clean it in your "make clean"
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $<

clean:
	rm -f src/*.o
	rm -f main
	rm -f tests/CUnit/*.o
	rm -f tests/graph_bin/graph*
	rm -f tests/result_bin/result*
	# ...

tests:
#	create the graph files for inputs of tests 
	python tests/necessary_python/create_graph.py --output tests/graph_bin/graph_def.bin
	python tests/necessary_python/create_graph.py --ntf tests/ntf_files/negative_cost.ntf --output tests/graph_bin/graph_neg.bin
	python tests/necessary_python/create_graph.py --ntf tests/ntf_files/zeros_everywhere.ntf --output tests/graph_bin/graph_0.bin

#   run the python code to compute the expected outputs of tests	
	python tests/necessary_python/sp.py tests/graph_bin/graph_def.bin -f tests/result_bin/result_def.bin
	python tests/necessary_python/sp.py tests/graph_bin/graph_neg.bin -f tests/result_bin/result_neg.bin

	# Do not forget the -lcunit flag
	echo "TODO: compile and execute the tests!"
	echo "TODO: take into account the 2 bin files already done, empty and corrupted"


# a .PHONY target forces make to execute the command even if the target already exists
.PHONY: clean tests
