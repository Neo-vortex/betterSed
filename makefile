all:
	gcc -std=c17 -o result *.c


run: $(OUT)
	./$(OUT)
