#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>
#include "dirent.h"
#include "MinHeap.h"
#include "Huffman.h"
#define MAX_FILENAME_LENGTH 256
#define INPUT_SIZE 32

void test_file(char *filename);
void read_table(FILE *file, MinHeap *heap);
int compare(const void *a, const void *b);
void decode_file(FILE *decoding, MinHeapNode* huffman, FILE* decoded);

int main(int argc, char *argv[]){
  setlocale(LC_ALL, "");
  if(argc < 2){
   printf("Please give a directory to decompress\n");
   exit(1);  
  }

  if(chdir(argv[1]) ==  -1){
    perror("Couldn't change directory\n");
    exit(1);
  }

  DIR *d;
  FILE *table;
  FILE *decoding;
  FILE *decoded;
  char **files = NULL;
  int count = 0;

  //Name of the encoded file
  char file_decoding[MAX_FILENAME_LENGTH];
  //Name of file table
  char file_table[MAX_FILENAME_LENGTH];
  //Name of the final decoded file
  char file_decoded[MAX_FILENAME_LENGTH];
  //To compare the filenames
  char table_filename[MAX_FILENAME_LENGTH];

  struct dirent *dir;
  d = opendir(".");
  if (d) {
    //Stores all the filenames
    while((dir = readdir(d))!= NULL){
      if(dir->d_type == DT_REG){
        files = realloc(files, (count + 1) * sizeof(char *));
        files[count] = strdup(dir->d_name);
        count++;
      }
    }

    //Sort the filenames alphabeticcaly
    qsort(files, count, sizeof(char *), compare);

    for(int i = 0; i < count; i++){

      const char *ext = strrchr(files[i],'.');

      if((!ext) || (ext == files[i])){
          printf("Not an encoded file\n");
          continue;
      }

      if(strstr(ext,".encoded")){
        memset(table_filename, 0, sizeof(table_filename));
        strncpy(table_filename, files[i], strlen(files[i]) - strlen(".encoded"));
        strcat(table_filename, ".table");

        memset(file_decoded, 0, sizeof(file_decoded));
        strncpy(file_decoded, files[i], strlen(files[i]) - strlen(".encoded"));
        strcat(file_decoded, ".decoded");

        // Check if the corresponding .table file exists
        int found = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(files[j], table_filename) == 0) {
              
                found = 1;
                MinHeap * heap = init_min_heap(1024);


                table = fopen(files[j],"r");
                decoding = fopen(files[i], "r");

                if(table == NULL){
                      perror("Couldnt open file\n");
                }else{
                      read_table(table, heap);
                      fclose(table);
                }
                
                if(decoding == NULL){
                  perror("Couldnt open file\n");
                }else{
                  MinHeapNode *huffman = create_huffman(heap);
                  decoded = fopen(file_decoded, "w");
                  decode_file(decoding,huffman,decoded);
                  fclose(decoding);
                  fclose(decoded);
                }

                //print_heap(heap); Un-comment for debugging purposes.
                destroy_min_heap(heap);

                break;
            }
        }
            
        if (!found) {
                printf("Error: Corresponding .table file not found for %s\n", files[i]);
                // Handle error or continue to next file
        }
      }
    }
    closedir(d);
 }
}

void test_file(char *filename){
  wint_t wc;
  unsigned int frequency;
  FILE *test_file = fopen(filename,"r");

  while(fscanf(test_file,"%u %u",&wc,&frequency) == 2){
    wprintf(L"Character is: %lc, Frequency is: %u\n", (wchar_t)wc, frequency);
  }
}

void read_table(FILE *file, MinHeap *heap){
  wint_t wc;
  unsigned int frequency;

  while(fscanf(file,"%u %u",&wc,&frequency) == 2){
    insert_decompression(heap, (wchar_t)wc, frequency);
  }
}

int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void decode_file(FILE *decoding, MinHeapNode* huffman, FILE* decoded){
  //Get number of bits
  int input_bits_size;
  fread(&input_bits_size, sizeof(int),1, decoding);
  input_bits_size -= INPUT_SIZE;


  int nbits = 0;
  unsigned char bits = 0;

  MinHeapNode *root = huffman;

  int output_count = 0;

  while (output_count < input_bits_size) {
    
        if (nbits == 0) {
            if (feof(decoding)) break;
            bits = fgetc(decoding);
            nbits = 8;
        }

        if (huffman->left == NULL && huffman->right == NULL) {
            fputwc(huffman->character, decoded);
            huffman = root;
            output_count++;
            if (output_count == input_bits_size)
                break;
        }else{
          nbits--;
          if (bits & (1 << (nbits))) {
            huffman = huffman->right;
          } else {
            huffman = huffman->left;
          }
        }
    }
}