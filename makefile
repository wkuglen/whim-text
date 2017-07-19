.DEFAULT_GOAL := test

build:
		gcc whim.c manager.c -o whim -lcurses

clean:
		rm -rf *.dSYM
			rm -rf *.out

test: build
		./whim
			
