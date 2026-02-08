





.PHONY: test
test:
	@gcc ./tests/main.c -o ./main
	./main
	@rm ./main
