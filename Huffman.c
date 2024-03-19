#include "MinHeap.h"
#include "Huffman.h"
#include <stdlib.h>
#include <stdio.h>

/*Para el top en el huffman coding usar la virgulilla ~*/
/*Pregunta un poco menos importante, esta deberia estar aqui o en el heap.h?
Nota: NO LLAMAR DESPUES DE generate_codes_list porque esa func ya utiliza esta*/
MinHeapNode *create_huffman(MinHeap *heap){
    MinHeapNode *left;
    MinHeapNode *right;
    MinHeapNode *new;
    while(heap->size != 1){
        left = remove_first(heap);
        right = remove_first(heap);

        new = create_node('~', left->frequency + right->frequency);
        new->left = left;
        new->right = right;
        new->isIntermediate = 1; // Igual a True en este caso

        insert_intermediate_node(heap, new);
    }
    return remove_first(heap);
}

int huffman_height(MinHeapNode *root){
    return huffman_height_aux(root);
}

int huffman_height_aux(MinHeapNode *root){
    if(root == NULL){
        return 0;
    }
    int left_height = huffman_height_aux(root->left);
    int right_height = huffman_height_aux(root->right);
    if(left_height> right_height){
        return 1 + left_height;
    }else{
        return 1 + right_height;
    }
}

HuffmanNode *init_huffman_node(int arr[], int num_digits, wchar_t character){
    HuffmanNode *huff = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    huff->character = character;
    huff->code = malloc(num_digits * sizeof(int));
    huff->next = NULL;
    huff->numdigits = num_digits;
    for(int i = 0; i < num_digits; i++){
        huff->code[i] = arr[i];
    }
    return huff;
}

HuffmanList* generate_codes_list(MinHeap *heap){
    HuffmanList *list = init_huffman_list();
    MinHeapNode *root = create_huffman(heap);
    int arr[huffman_height(root)];
    int top = 0;
    generate_codes_list_aux(root, arr, top, list);
    return list;
}

void generate_codes_list_aux(MinHeapNode* root, int arr[], int top , HuffmanList *list){
    if(root->left){
        arr[top] = 0;
        generate_codes_list_aux(root->left, arr, top + 1, list);
    }
    if(root->right){
        arr[top] = 1;
        generate_codes_list_aux(root->right, arr, top + 1, list);
    }
    if(is_end_node(root)){
        HuffmanNode *new_code  = init_huffman_node(arr, top, root->character);
        insert_huffman_node(list, new_code);
    }
}

int is_end_node(MinHeapNode *node){
    return !(node->left) && !(node->right); 
}

void destroy_huffman(MinHeapNode* root){
    if(root->left != NULL){
        destroy_huffman(root);
    }
    if(root->right != NULL){
        destroy_huffman(root);
    }
    free(root);
}

HuffmanList *init_huffman_list(){
    HuffmanList *list = (HuffmanList*)malloc(sizeof(HuffmanList));
    list->data = NULL;
    return list;
}

void insert_huffman_node(HuffmanList *list, HuffmanNode *node){
    struct HuffmanNode* data = list->data;
    if(data==NULL){
         list->data = node;
    }else{
        while(data->next != NULL){
            data = data->next;
        }
        data->next = node;
    }
}

void print_huffman_list(HuffmanList *list){
     if (list->data == NULL) {
        printf("Huffman list is empty.\n");
        return;
    }

    struct HuffmanNode* currentNode = list->data;
    while (currentNode != NULL) {
        printf("Character: %lc\n",currentNode->character);
        printf("[");
        for(int i = 0; i < currentNode->numdigits; i++){
            printf("%i,", currentNode->code[i]);
        }
        printf("]\n");
        currentNode = currentNode->next;
    }
    printf("\n");
}

void huffman_in_order_traversal(MinHeapNode *root, FILE* file){
    if(root != NULL){
        huffman_in_order_traversal(root->left,file);
        fwprintf(file,L"%lc",root->character);
        huffman_in_order_traversal(root->right, file);
    }
}

void huffman_post_order_traversal(MinHeapNode *root, FILE* file){
    if(root != NULL){
        huffman_post_order_traversal(root->left, file);
        huffman_post_order_traversal(root->right, file);
        fwprintf(file,L"%lc",root->character);
    }
}


void print_huffman_to_file(HuffmanList *list, FILE* file, char *filename){

}