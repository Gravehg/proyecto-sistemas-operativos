CC=gcc

compiletest: 
	$(CC) sequentialreadandcompress.c Huffman.c MinHeap.c -o test

compiledebug:
	$(CC) -g sequentialreadandcompress.c Huffman.c MinHeap.c -o test

compiledecompress:
	$(CC) sequentialdecompress.c Huffman.c MinHeap.c -o decompress

compiledecompressdebug:
	$(CC) -g sequentialdecompress.c Huffman.c MinHeap.c -o decompress
clear:
	rm test