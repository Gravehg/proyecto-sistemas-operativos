#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include "dirent.h"
#include "MinHeap.h"
#include "Huffman.h"
#define MAX_FILENAME_LENGTH 256
#define INPUT_SIZE 32


typedef struct ProcessArgs{
    char *encoded_filename;
    char *table_filename;
    char *decoded_filename;
} ProcessArgs;

void test_file(char *filename);
void read_table(FILE *file, MinHeap *heap);
int compare(const void *a, const void *b);
void decode_file(FILE *decoding, MinHeapNode* huffman, FILE* decoded);
void *decode_thread(void *args);



int total_processes = 0;

int main(int argc, char *argv[]){
  setlocale(LC_ALL, "");

  if(argc < 2){
   printf("Please give the directory where the .tgz file to decompress is\n");
   exit(1);  
  }

  if(argc < 3){
    printf("Please give the .tgz file name to decompress\n");
    exit(1);
  }

  if(chdir(argv[1]) ==  -1){
    perror("Couldn't change directory\n");
    exit(1);
  }

  struct stat stat_buffer;
  if(!(stat (argv[2], &stat_buffer) == 0)){
    printf("The file doesn't exist!\n");
    exit(1);
  }

  //To store the system call
  char buff[2000];
  sprintf(buff, "tar zxf %s", argv[2]);
  int err;
  if ((err = system(buff)))
  {
      printf("There was an error (%i)\n", err);
      return 1;
  }
  char buff_directory_name[1024];

  strncpy(buff_directory_name, argv[2], strlen(argv[2]) - strlen(".tgz"));
  //Why, is it because this operation takes some time
  buff_directory_name[strlen(argv[2]) - strlen(".tgz")] = '\0';

  if(chdir(buff_directory_name) ==  -1){
    perror("Couldn't change to .tgz directory\n");
    exit(1);
  }



  DIR *d;
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
                ProcessArgs *arg = (ProcessArgs*)malloc(sizeof(ProcessArgs));
                arg->table_filename = strdup(files[j]);
                arg->encoded_filename = strdup(files[i]);
                arg->decoded_filename = strdup(file_decoded);
                total_processes++;
                pid_t pid =  fork();
                if(pid == -1){
                    perror("Error forking process\n");
                    exit(1);
                }else if(pid == 0){
                    decode_thread((void*)arg);
                    exit(0);
                }

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
  int status;
  while (wait(&status) > 0);
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

  //printf("Input bits size is: %i\n", input_bits_size);


  int nbits = 0;
  unsigned char bits = 0;

  MinHeapNode *root = huffman;

  int output_count = 0;

  while (input_bits_size > 0) {
    
        if (nbits == 0) {
            if (feof(decoding)) break;
            bits = fgetc(decoding);
            nbits = 8;
        }

        if (huffman->left == NULL && huffman->right == NULL) {
            fputwc(huffman->character, decoded);
            huffman = root;
        }else{
          nbits--;
          input_bits_size--;
          if (bits & (1 << (nbits))) {
            huffman = huffman->right;
          } else {
            huffman = huffman->left;
          }
        }
    }
    fputwc(huffman->character,decoded);
}


void *decode_thread(void *args){
    struct ProcessArgs *thread_args = (struct ProcessArgs *)args;
    char *encoded_file = thread_args->encoded_filename;
    char *table_file = thread_args->table_filename;
    char *decoded_file = thread_args->decoded_filename;
    
    FILE *table = fopen(table_file,"r");
    FILE *decoding = fopen(encoded_file, "r");
    FILE *decoded = fopen(decoded_file, "w");

    if (table == NULL || decoding == NULL || decoded == NULL) {
        perror("Couldn't open file\n");
        exit(1);
    }

    MinHeap * heap = init_min_heap(1024);
    read_table(table, heap);
    fclose(table);
    
    MinHeapNode *huffman = create_huffman(heap);
    destroy_min_heap(heap);

    decode_file(decoding,huffman,decoded);

    fclose(decoding);
    fclose(decoded);

    free(decoded_file);
    free(table_file);
    free(encoded_file);
}