all:
	g++ -o $(f) $(f).cpp -std=c++11
gdb:
	g++ -o $(f) $(f).cpp -std=c++11 -g