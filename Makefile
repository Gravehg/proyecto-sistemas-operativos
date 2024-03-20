CC=gcc

compiletest: 
	$(CC) sequentialreadandcompress.c Huffman.c MinHeap.c -o test

compiledebug:
	$(CC) -g sequentialreadandcompress.c Huffman.c MinHeap.c -o test
clear:
	rm test