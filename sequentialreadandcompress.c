#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>
#include "dirent.h"
#include "MinHeap.h"
#include "Huffman.h"
#define OUTPUT_SIZE 32

void encode_and_write(FILE *to_encode,FILE* encoded, HuffmanList *list, 
int size_of_output);

int main(int argc, char*argv[]){
  
  setlocale(LC_ALL, "");
  if(argc < 2){
   printf("Please give a directory to compress\n");
   exit(1);  
  }

  if(chdir(argv[1]) ==  -1){
    perror("Couldn't change directory\n");
    exit(1);
  }

  DIR *d;
  FILE *to_encode;
  FILE *to_encode_table;
  FILE *encoded;
  struct dirent *dir;
  //Name of original file
  char file_name[256];
  //Name of frequency file
  char file_frequency[256];
  //Name of encoded file
  char file_encoded[256];
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
       if (dir->d_type == DT_REG)
      {
        
        strcpy(file_name,dir->d_name);
        strcpy(file_frequency, file_name);
        strcpy(file_encoded, file_name);
        strcat(file_encoded,".encoded");
        strcat(file_frequency,".table");

        //Opens original file
        to_encode = fopen(file_name, "r");
        //Opens table file
        to_encode_table = fopen(file_frequency, "w");

        if(to_encode == NULL){
          perror("Error trying to open file\n");
          exit(1);
        }

        wint_t wc;
        MinHeap *heap = init_min_heap(1024);
        while((wc = fgetwc(to_encode)) != WEOF){
            insert_node(heap,(wchar_t)wc);
        }
        
        //Print the heap for debugging
        //print_heap(heap);

        //Generates .encoded files
        print_to_file(to_encode_table, heap);
        fclose(to_encode_table);

        HuffmanList *huffman = generate_codes_list(heap);

        /*Print the huffman list -!Use when debugging*/
        //print_huffman_list(huffman);

        //The OUTPUT_SIZE is added to save space for 32 additional bits that have the
        //size of the output
        int size_of_output = calculate_size(huffman) + OUTPUT_SIZE;

        encoded = fopen(file_encoded,"wb");

        rewind(to_encode);

        encode_and_write(to_encode,encoded,huffman, size_of_output);

        fclose(to_encode);
        fclose(encoded);
        destroy_min_heap(heap);
        destroy_huffman_list(huffman);
      }
    }
    closedir(d);
  }


  //print_heap(heap);
  //HuffmanList* list = generate_codes_list(heap);
  //MinHeapNode *root = create_huffman(heap);
  //print_huffman_list(list);

  //huffman_in_order_traversal(root,new_file);

    // Read the file character by character

  return 0;
}



void encode_and_write(FILE *to_encode, FILE *encoded, HuffmanList *list, int size_of_output) {
    // Write the size of the output to the encoded file
    fwrite((const void*)&size_of_output, sizeof(int), 1, encoded);
    
    int nbits = 0;
    unsigned char bits = 0;

    wint_t wc;
    while ((wc = fgetwc(to_encode)) != WEOF) {
        HuffmanNode *current_node = list->data;
        while (current_node != NULL && current_node->character != wc) {
            current_node = current_node->next;
        }
        if (current_node == NULL) {
            fprintf(stderr, "Error: Character not found in Huffman list.\n");
            return;
        }
        // Write the Huffman code to the output file
        for (int i = 0; i < current_node->numdigits; i++) {
            bits = (bits << 1) | current_node->code[i]; // Append the next bit to 'bits'
            nbits++;
            if (nbits == 8) {
                fputc(bits, encoded);
                bits = 0;
                nbits = 0;
            }
        }
    }
    // Flush any remaining bits
    if (nbits > 0) {
        bits <<= (8 - nbits); // Shift remaining bits to the leftmost position
        fputc(bits, encoded);
    }
}