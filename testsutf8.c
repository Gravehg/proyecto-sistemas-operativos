#include <stdio.h>
#include <stdlib.h>
#include "MinHeap.h"
#include "Huffman.h"

int main() {
   MinHeap *heap = init_min_heap(11);
   insert_node(heap, 'A');
   insert_node(heap, 'B');
   for(int i = 0; i < 100; i++){
    insert_node(heap, 'C');
   }
   if(contains_char(heap,'A')){
    printf("It contains the char you're looking for\n");
   }
   if(!contains_char(heap, 'D')){
      printf("It doesnt have the char you're looking for\n");
   }
   //print_heap(heap);
   HuffmanList *list = generate_codes_list(heap);
   print_huffman_list(list);

   destroy_min_heap(heap);
}
