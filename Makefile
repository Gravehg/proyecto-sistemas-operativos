CC=gcc

compilecompress: 
	$(CC) sequentialreadandcompress.c Huffman.c MinHeap.c -o compress

compiledebug:
	$(CC) -g sequentialreadandcompress.c Huffman.c MinHeap.c -o compress

compiledecompress:
	$(CC) sequentialdecompress.c Huffman.c MinHeap.c -o decompress

compiledecompressdebug:
	$(CC) -g sequentialdecompress.c Huffman.c MinHeap.c -o decompress

compilecompressthreads:
	$(CC) threads_readandcompress.c Huffman.c MinHeap.c -o compress_threads

compiledecompressthreads:
	$(CC) threads_decompress.c Huffman.c MinHeap.c -o decompress_threads


compiledecomthreadsdebug:
	$(CC) -g threads_decompress.c Huffman.c MinHeap.c -o decompress_threads
	
clear:
	rm test