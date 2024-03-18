#ifndef HUFFMAN_H
#define HUFFMAN_H
#include "MinHeap.h"
#include <wchar.h>

typedef struct HuffmanNode{
    int *code;
    wchar_t character;
    int numdigits;
    struct HuffmanNode *next;
}HuffmanNode;

typedef struct HuffmanList{
    struct HuffmanNode *data;
} HuffmanList;

MinHeapNode* create_huffman(MinHeap *heap);
int huffman_height(MinHeapNode *root);
int huffman_height_aux(MinHeapNode *root);
HuffmanNode *init_huffman_node(int arr[], int num_digits, wchar_t character);
HuffmanList* generate_codes_list(MinHeap *heap);
void generate_codes_list_aux(MinHeapNode* root, int arr[], int top , HuffmanList *list);
int is_end_node(MinHeapNode *node);
void destroy_huffman(MinHeapNode* root);

//Managing the list
HuffmanList *init_huffman_list();
void insert_huffman_node(HuffmanList *list, HuffmanNode *node);
void print_huffman_list(HuffmanList *list);

#endif