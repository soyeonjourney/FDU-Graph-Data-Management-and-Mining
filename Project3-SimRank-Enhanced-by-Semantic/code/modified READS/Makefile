all:
	g++ reads*.h reads*.cpp timer.h -O3 -w -std=c++11 test.cpp -I .
test:
	./a.out test.data 67 30
clean:
	rm -f a.out *.txt *.gch
