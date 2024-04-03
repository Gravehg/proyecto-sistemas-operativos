CC=gcc

compilecompress: 
	$(CC) sequentialreadandcompress.c Huffman.c MinHeap.c -o compress -lrt

compiledebug:
	$(CC) -g sequentialreadandcompress.c Huffman.c MinHeap.c -o compress -lrt

compiledecompress:
	$(CC) sequentialdecompress.c Huffman.c MinHeap.c -o decompress -lrt

compiledecompressdebug:
	$(CC) -g sequentialdecompress.c Huffman.c MinHeap.c -o decompress -lrt

compilecompressthreads:
	$(CC) threads_readandcompress.c Huffman.c MinHeap.c -o compress_threads -lrt

compiledecompressthreads:
	$(CC) threads_decompress.c Huffman.c MinHeap.c -o decompress_threads -lrt


compiledecomthreadsdebug:
	$(CC) -g threads_decompress.c Huffman.c MinHeap.c -o decompress_threads -lrt

compileprocesscompress:
	$(CC) fork_compress.c Huffman.c MinHeap.c -o compress_process -lrt

compileprocessdecompress:
	$(CC) fork_decompress.c Huffman.c MinHeap.c -o decompress_process -lrt

compileall: compilecompress compiledecompress compilecompressthreads compiledecompressthreads compileprocesscompress compileprocessdecompress
	
clear:
	rm compress decompress compress_threads decompress_threads compress_process decompress_process