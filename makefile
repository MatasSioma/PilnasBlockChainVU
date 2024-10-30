main: ./source/generate.cpp ./source/block.cpp
	g++ -O1 ./source/generate.cpp ./source/block.cpp ./hash/hash.cpp -o block

clear:
	rm block