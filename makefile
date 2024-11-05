main: ./source/generate.cpp ./source/block.cpp ./source/merkle.cpp
	g++ -o block ./source/generate.cpp ./source/block.cpp ./hash/hash.cpp ./source/merkle.cpp -lbitcoin-system

testmerkle: ./source/merkle.cpp
	g++ -o merkle-test ./source/merkle.cpp -lbitcoin-system

clear:
	rm block