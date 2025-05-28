all: clean build run

clean:
	@rm -rf a.out

build:
	@gcc src/*.c

run:
	@./a.out

.PHONY: all clean build run
