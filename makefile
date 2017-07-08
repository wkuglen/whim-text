.DEFAULT_GOAL := test

build:
		gcc -g whim.c -o whim -lcurses

clean:
		rm -rf *.dSYM
			rm -rf *.out

test: build
		./whim
			
