#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include "MinHeap.h"
#include "Huffman.h"

int main(int argc, char*argv[]){
  
  setlocale(LC_ALL, "");
  if(argc < 2){
   printf("Please give a directory to compress\n");
   exit(1);  
  }

  /**if(chdir(argv[1]) != 0){
    perror("Couldn't change directory");
    exit(1);
  }**/

  FILE *file;
  FILE *new_file;
  file = fopen("Frankenstein", "r"); // Open the file in read mode
  new_file = fopen("newfile.txt","w");

  if (file == NULL) {
      printf("Error opening file.\n");
      return 1;
  }
  wint_t wc;
   MinHeap *heap = init_min_heap(1024);
  while((wc = fgetwc(file)) != WEOF){
    insert_node(heap,(wchar_t)wc);
  }
  //print_heap(heap);
  HuffmanList* list = generate_codes_list(heap);
  print_huffman_list(list);

    // Read the file character by character

  return 0;
}