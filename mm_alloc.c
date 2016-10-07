// mm_alloc.c
#include "mm_alloc.h"
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>

/** mm_malloc - allocate a block of at least size bytes **/
void*
mm_malloc(size_t size) {
  // TODO
  //if first allocation
  if(first){
    void* mem_begin = sbrk((2^MAX_EXP));
    void* test = (void*)-1;
    if(mem_begin == test){
      return NULL;
    }
    head->size = (2^MAX_EXP);
    head->free = true;
    head->data = NULL;
    head->next = NULL;
    head->buddy = NULL;

    for (size_t i = 0; i < MAX_EXP; i++) {
      head->merged_buddy[i] = NULL;
    }
    first = false;
    structSize = sizeof(*head);
  }

  //buddy allocation
  size_t allocation_size = size + structSize;
  //if the curr block is large enough to store the data
  block* curr = head;
  if(curr->size>=allocation_size){
    //if the curr block is free
    if(curr->free == true){
      //if curr->size/2 fits the data
      while(((curr->size)/2)>=allocation_size){
        block* buddy;
        buddy->size = ((curr->size)/2);
        buddy->free = true;
        buddy->data = NULL;
        buddy->next = curr->next;
        buddy->buddy = curr;

        for (size_t i = 0; i < MAX_EXP; i++) {
          if((buddy->merged_buddy[i] == NULL)){
            buddy->merged_buddy[i] = buddy->buddy;
          }
        }
        curr->size = ((curr->size)/2);
        curr->free = true;
        curr->data = NULL;
        curr->next = buddy;
        curr->buddy = buddy;
        for (size_t i = 0; i < MAX_EXP; i++) {
          if(!(curr->merged_buddy[i] == NULL)){
            curr->merged_buddy[i] = curr->buddy;
          }
        }
      }
      curr->free = false;
      return curr;
    }else if(curr->free != true){

    }
  }

  return NULL;
}

/** mm_free - marks a data block as free **/
void
mm_free(void* ptr) {
    // TODO
}

/** mm_realloc - changes the data block to have the specified size **/
void*
mm_realloc(void* ptr, size_t size) {
    // TODO
    return NULL;
}

/*------------------------------------*\
|            DEBUG FUNCTIONS           |
\*------------------------------------*/

/** print_block - prints the metadata of a block **/
void
print_block(struct block* b) {
  if(!b) {
    printf("NULL block\n");
  }
  else {
    int i = 0;
    printf("Strt = %lx\n",(unsigned long) b);
    printf("Size = %lu\n",b->size);
    printf("Free = %s\n",(b->free)?"true":"false");
    printf("Data = %lx\n",(unsigned long) b->data);
    printf("Next = %lx\n",(unsigned long) b->next);
    printf("Buddy = %lx\n",(unsigned long) b->buddy);
    printf("Merged Buddies = ");
    while(b->merged_buddy[i] && i < MAX_EXP) {
        printf("%lx, ",(unsigned long) b->merged_buddy[i]);
        i++;
    }
    printf("\n\n");
  }
}

/** print_list - prints the metadata of all blocks **/
void
print_list() {
    struct block* curr = head;
    printf("--HEAP--\n");
    if(!head) printf("EMPTY\n");
    while(curr) {
        print_block(curr);
        curr = curr->next;
    }
    printf("--END--\n");
}
