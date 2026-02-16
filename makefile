





.PHONY: test
test:
	@clang -std=c99 -Wall ./tests/$(TEST)_test.c -o ./main
	./main
	@rm ./main
