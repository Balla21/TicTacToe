p1: pro1.cpp const.h
	g++ -g pro1.cpp -o p1

p2: pro2.cpp const.h
	g++ -g pro2.cpp -o p2

clean:
	rm p1 p2

zip:
	zip named_pipe.zip *.cpp *.h Makefile