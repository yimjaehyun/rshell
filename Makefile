all : src/main.cpp $(H_FILES)
	mkdir -p bin
	g++ src/main.cpp -o bin/rshell -Wall -Werror -ansi -pedantic

rshell : src/main.cpp $(H_FILES)
	mkdir -p bin
	g++ src/main.cpp -o bin/rshell -Wall -Werror -ansi -pedantic

clean :
	rm -rf bin 
