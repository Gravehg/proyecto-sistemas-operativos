#ifndef MINHEAP_H
#define MINHEAP_H

typedef struct MinHeapNode {
    /*Revisar esto, como puedo hacerlo por aparte, es mejor hacerlo por aparte?
    Haciendo otra estructura que guarde las cosas?*/
    struct MinHeapNode *left;
    struct MinHeapNode *right;
    char character;
    unsigned int frequency;
    int isIntermediate;
} MinHeapNode;

typedef struct MinHeap{
    struct MinHeapNode **heap;
    unsigned int size;
    unsigned int capacity;
}MinHeap;


MinHeap *init_min_heap(int capacity);
MinHeap *realloc_heap(MinHeap *heap);
MinHeap *destroy_min_heap(MinHeap *heap);
int left_child(int pos);
int right_child(int pos);
int parent(int pos);
int is_leaf(int pos , MinHeap *heap);
MinHeapNode *create_node(char character, int frequency);
MinHeapNode *destroy_node(MinHeapNode *node);
void swap(MinHeap *heap, int pos_one, int pos_two);
void siftUp(MinHeap *heap , int pos);
void siftDown(MinHeap*heap, int pos);
int greater_child(MinHeap* heap, int pos);
void insert_node(MinHeap *heap, char character);
MinHeapNode *remove_first(MinHeap *heap);
MinHeapNode* remove_heap_element(MinHeap *heap,int pos);
int get_size(MinHeap *heap);
MinHeapNode *first(MinHeap *heap);
int contains_char(MinHeap *heap, char character);
void print_heap(MinHeap *heap);
int get_element_pos(MinHeap *heap, char character);
void insert_intermediate_node(MinHeap *heap, MinHeapNode *node);

#endif