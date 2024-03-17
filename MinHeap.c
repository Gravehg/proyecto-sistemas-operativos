#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MinHeap.h"

MinHeap *init_min_heap(int capacity){
    struct MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->heap = (MinHeapNode**)malloc(capacity * sizeof(MinHeapNode *));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

MinHeap *realloc_heap(MinHeap *heap){
    heap->capacity *= 2;
        // Reallocate memory for the heap array
    heap->heap = realloc(heap->heap, heap->capacity * sizeof(MinHeapNode *));
    if (heap->heap == NULL) {
            printf("Error reallocating memory.\n");
            return NULL;
    }
}

MinHeap *destroy_min_heap(MinHeap *heap){
    free(heap);
}

int left_child(int pos){
    return 2 * pos + 1;
}

int right_child(int pos){
    return 2 * pos + 2;
}

int parent(int pos){
    return (pos - 1) / 2;
}

int is_leaf(int pos , MinHeap *heap){
    return left_child(pos)>= heap->size;
}

MinHeapNode *create_node(char character, int frequency){
    struct MinHeapNode *temp = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    temp->character = character;
    temp->frequency = frequency;
    temp->left = temp->right = NULL;
    temp->isIntermediate = 0;
    return temp;
}

MinHeapNode *destroy_node(MinHeapNode *node){
    free(node);
}


void swap(MinHeap *heap, int pos_one, int pos_two){
    MinHeapNode *temp = heap->heap[pos_one];
    heap->heap[pos_one] = heap->heap[pos_two];
    heap->heap[pos_two] = temp;
}

void siftUp(MinHeap *heap , int pos){
    while(pos != 0 && heap->heap[pos]->frequency <  heap->heap[parent(pos)]->frequency){
        swap(heap, pos, parent(pos));
        pos = parent(pos);
    }
}

void siftDown(MinHeap*heap, int pos){
    while(!is_leaf(pos, heap) && heap->heap[greater_child(heap, pos)]->frequency < heap->heap[pos]->frequency){
        int child = greater_child(heap, pos);
        swap(heap, pos, child);
        pos = child;
    }
}

int greater_child(MinHeap* heap, int pos){
    if(right_child(pos) >= heap->size || 
            heap->heap[left_child(pos)]->frequency < heap->heap[right_child(pos)]->frequency){
            return left_child(pos);
    }
    return right_child(pos);
}

void insert_node(MinHeap *heap, char character){
    if(heap->size >= heap->capacity){
        realloc_heap(heap);
    }
    if(contains_char(heap, character)){
        int pos = get_element_pos(heap,character);
        heap->heap[pos]->frequency++;
        siftUp(heap,pos);
    }else{
        MinHeapNode *new_node = create_node(character, 1);
        heap->heap[heap->size] = new_node;
        siftUp(heap, heap->size);
        heap->size++;
    }
}

void insert_intermediate_node(MinHeap *heap, MinHeapNode *node){
     if(heap->size >= heap->capacity){
        realloc_heap(heap);
    }
    heap->heap[heap->size] = node;
    siftUp(heap, heap->size);
    heap->size++;
}

MinHeapNode* remove_heap_element(MinHeap *heap, int pos){
    if(heap->size == 0){
        printf("Invalid index but why\n");
        exit(1);
    }
    if(pos < 0 || pos >= heap->size){
        printf("Invalid index (2) \n");
        exit(1);
    }
    swap(heap, pos, heap->size - 1);
    heap->size--;
    if(heap->size > 0)
        siftDown(heap, pos);
    return heap->heap[heap->size];
}

MinHeapNode *remove_first(MinHeap *heap){
    return remove_heap_element(heap,0);
}

int get_size(MinHeap *heap){
    return heap->size;
}

int get_element_pos(MinHeap *heap, char character){
    for(int i = 0; i < heap->size; i++){
        if(strcmp(&(heap->heap[i]->character),&(character)) == 0)
            return i;
    }
    return -1;
}

MinHeapNode *first(MinHeap *heap){
    return heap->heap[0];
}

int contains_char(MinHeap *heap, char character){
    for(int i = 0; i < heap->size; i++){
        if(strcmp(&(heap->heap[i]->character),&(character)) == 0)
            return 1;
    }
    return 0;
}

void print_heap(MinHeap *heap){
    printf("[ ");
    for(int i = 0; i < heap->size; i++){
        printf("( ");
        printf("%c, ", heap->heap[i]->character);
        printf("%u)", heap->heap[i]->frequency);
    }
    printf("]\n");
}