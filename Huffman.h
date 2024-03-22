#ifndef HUFFMAN_H
#define HUFFMAN_H
#include "MinHeap.h"
#include <stdio.h>
#include <wchar.h>

typedef struct HuffmanNode{
    int *code;
    wchar_t character;
    int numdigits;
    unsigned int frequency;
    struct HuffmanNode *next;
}HuffmanNode;

typedef struct HuffmanList{
    struct HuffmanNode *data;
} HuffmanList;

MinHeapNode* create_huffman(MinHeap *heap);
int huffman_height(MinHeapNode *root);
int huffman_height_aux(MinHeapNode *root);
HuffmanNode *init_huffman_node(int arr[], int num_digits, wchar_t character, unsigned int frequency);
HuffmanList* generate_codes_list(MinHeap *heap);
void generate_codes_list_aux(MinHeapNode* root, int arr[], int top , HuffmanList *list);
int is_end_node(MinHeapNode *node);
void destroy_huffman(MinHeapNode* root);
void huffman_in_order_traversal(MinHeapNode *root, FILE* file);
void print_in_order_traversal(MinHeapNode *root);
void printHuffmanTree(MinHeapNode* root, int depth);

//Managing the list
HuffmanList *init_huffman_list();
void insert_huffman_node(HuffmanList *list, HuffmanNode *node);
void print_huffman_list(HuffmanList *list);
void print_huffman_to_file(HuffmanList *list, FILE* file, char *filename);
void huffman_post_order_traversal(MinHeapNode *root, FILE* file);
int calculate_size(HuffmanList* list);

#endif