The input data of this part of program should be like:

0 1 1.4
2 0 0.5
1 2 2.1

The first two entries x, y of each line represent the edge (x, y) (with direction), and the last entry of the line represents the weight of this edge (No need to normalize it). This weight is used for sampling in-neighbors while constructing offline index
To set up the program, type "make all".
To test the program, type "make test". You could change the data file and query node in Makefile. Here is an example of testing command:
	
./a.out test.data 67 63

The first input of the program is the data file name, the second input is the total nodes in the graph, the last input is the index of query node
And for test.data, the edge itself is bidirectional, so I open the "DOUBLE" switch in reads.cpp, for regular graph, you could set "DOUBLE" 0.
"make clean" will clean all the .txt, .gch and the program itself.
