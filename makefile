main: ./source/block.cpp
	g++ ./source/block.cpp ./source/generate.cpp ./hash/hash.cpp -o block

generate: ./source/generate.cpp ./source/block.cpp
	g++ ./source/generate.cpp ./source/block.cpp ./hash/hash.cpp -o generate

clear:
	rm block
	rm generate
