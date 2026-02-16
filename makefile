





.PHONY: test
test:
	@clang -std=c99 -Wall ./tests/main.c -o ./main
	./main
	@rm ./main
