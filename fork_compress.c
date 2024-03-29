#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "errno.h"
#include "dirent.h"
#include "time.h"
#include "MinHeap.h"
#include "Huffman.h"
#define OUTPUT_SIZE 32


typedef struct ProcessArgs {
    char *filename;
    char *directoryname;
} ProcessArgs;



void encode_and_write(FILE *to_encode,FILE* encoded, HuffmanList *list, 
int size_of_output);
void *compress_file(void * args);

int main(int argc, char*argv[]){
  struct timespec start, end;
  long long time;
  clock_gettime(CLOCK_MONOTONIC, &start);

  setlocale(LC_ALL, "");
  if(argc < 2){
   printf("Please give a directory to compress\n");
   exit(1);  
  }

  if(chdir(argv[1]) ==  -1){
    perror("Couldn't change directory\n");
    exit(1);
  }

  //Buffer for the new directory name
  char directory_name[256];
  strcpy(directory_name, argv[1]);
  strcat(directory_name,"_compressed");

  if(mkdir(directory_name,0777) == - 1){
    perror("Error");
    exit(1);
  }


  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
       if (dir->d_type == DT_REG)
      {
        struct ProcessArgs *args = malloc(sizeof(struct ProcessArgs));
        args->filename = strdup(dir->d_name);
        args->directoryname = strdup(directory_name);
        pid_t pid = fork();   
        if(pid == -1){
            perror("Fork failed\n");
            exit(1);
        }else if(pid == 0){
            compress_file((void*)args);
            exit(0);
        }
      }
    }
    closedir(d);
  }

  int status;
  while (wait(&status) > 0);
  

  //Create the .tgz file
  char buff [1000];
  sprintf(buff,"tar -zcf %s.tgz %s/",directory_name, directory_name);
  system(buff);
  //Remove the directory and its contents
  char remove_dir_call[1000];
  sprintf(remove_dir_call, "rm -r %s", directory_name);
  system(remove_dir_call);
  
  clock_gettime(CLOCK_MONOTONIC, &end);
  time = 1e9 * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);
  printf("Elapsed time: %lld nanoseconds\n", time);
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

void* compress_file(void * args){
    struct ProcessArgs *thread_args = (struct ProcessArgs *)args;
    char *file_name = thread_args->filename;
    char *directory_name = thread_args->directoryname;
    free(args); // Free memory allocated for arguments
    //Copia el nombre original del archivo a file_name
    //Luego copia ese mismo nombre al file_frequency
    //Luego copia ese mismo nombre al file_encoded

    char file_frequency[256];
    char file_encoded[256];
    strcpy(file_frequency, directory_name);
    strcpy(file_encoded, directory_name);
    strcat(file_frequency,"/");
    strcat(file_encoded, "/");
    strcat(file_frequency, file_name);
    strcat(file_encoded, file_name);


    strcat(file_encoded,".encoded");
    strcat(file_frequency,".table");

    FILE *to_encode;
    FILE *to_encode_table;
    FILE *encoded;

    //Opens original file
    to_encode = fopen(file_name, "r");
    //Opens table file
    to_encode_table = fopen(file_frequency, "w");

    if(to_encode == NULL || to_encode_table == NULL){
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