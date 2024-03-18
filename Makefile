CC=gcc

compiletest: 
	$(CC) sequentialreadandcompress.c Huffman.c MinHeap.c -o test

clear:
	rm test