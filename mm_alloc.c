// mm_alloc.c
#include "mm_alloc.h"
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

/** mm_malloc - allocate a block of at least size bytes **/
void*
mm_malloc(size_t size) {
  // TODO
  //if first allocation
  if(size == (size_t)0){
    return NULL;
  }
  if(head==NULL){
    head = (struct block*)(sbrk((size_t)pow(2,MAX_EXP)+(size_t)(MAX_EXP*sizeof(struct block))));
    head->size = (size_t)pow(2,MAX_EXP);
    head->free = true;
    head->data = (void *)((size_t)head + (size_t)(sizeof(struct block)));
    head->next = NULL;
    head->buddy = NULL;
    for (size_t i = 0; i < MAX_EXP; i++) {
      head->merged_buddy[i] = NULL;
    }
  }

  size_t alloc_size = (size_t)((size_t)size + (size_t)sizeof(struct block));
  struct block* curr = head;
  while((curr->free == false)||((size_t)(curr->size)<alloc_size)){
    if(curr->next == NULL){
      return NULL;
    }
    curr = curr->next;
  }
  while((curr->free == true)&&((size_t)(curr->size/2)>=alloc_size)){
    curr->size = (size_t)((curr->size)/2);
    struct block* buddy = (struct block*)((size_t)curr + (size_t)(curr->size));
    buddy->size = (size_t)(curr->size);
    buddy->free = true;
    buddy->data = (void*)((size_t)buddy + (size_t)sizeof(struct block));
    buddy->next = curr->next;
    buddy->buddy = curr;
    for(size_t i = 0; i < MAX_EXP; i++){
      if(curr->merged_buddy[i] == NULL) {
        curr->merged_buddy[i] = curr->buddy;
        break;
      }
    }
    curr->buddy = buddy;
    curr->next = buddy;
    for (size_t i = 0; i < MAX_EXP; i++) {
      buddy->merged_buddy[i] = curr->merged_buddy[i];
    }
    //print_list();
  }

  if((curr->free == true)&&((size_t)(curr->size)>=(size_t)alloc_size)){
    curr->free = false;
    //print_list();
    return curr->data;
  }
  return NULL;
}

/** mm_free - marks a data block as free **/
void
mm_free(void* ptr) {
    // TODO
    if(ptr!=NULL){
      struct block* curr = (struct block*)((size_t)ptr - (size_t)sizeof(struct block));
      curr->free = true;
      struct block* lowerbd = NULL;
      if(curr->buddy->free){
        if(((size_t)(curr->buddy))>((size_t)curr)){
          lowerbd = curr;
        }else{
          lowerbd = curr->buddy;
        }
        lowerbd->size = (size_t)(2*(lowerbd->size));
        size_t endBuddy = -1;
        for (size_t i = 0; i < MAX_EXP; i++) {
          if(lowerbd->merged_buddy[i]==NULL){
            endBuddy = i-1;
            break;
          }
        }

        for (size_t i = 0; i <= endBuddy+1; i++) {
          lowerbd->buddy = lowerbd->merged_buddy[endBuddy-i];
          curr = lowerbd;
          if(curr->buddy->free == true){
            if(((size_t)(curr->buddy))>((size_t)curr)){
              lowerbd = curr;
            }else{
              lowerbd = curr->buddy;
            }
            lowerbd->size = (size_t)(2*(lowerbd->size));
            lowerbd->merged_buddy[endBuddy-i] = NULL;
	    //printf("End list: %p\n",lowerbd->merged_buddy[endBuddy-i]);
	    //printf("End list buddy: %p\n",lowerbd->merged_buddy[endBuddy-i]);
            lowerbd->next = lowerbd->next->next;
          }else{
	           curr->merged_buddy[endBuddy-i] = NULL;
	           curr->next = curr->next->next;
	    //printf("break\n");
            break;
          }
        }
      }
    }
    //print_list();
}

/** mm_realloc - changes the data block to have the specified size **/
void*
mm_realloc(void* ptr, size_t size) {
    // TODO

    if(ptr == NULL){
      return mm_malloc(size);
    }
    struct block* curr = (struct block*)((size_t)ptr - (size_t)sizeof(struct block));

    if((curr->size>=size)&&(curr->size/2<size)){
      return ptr;
    }else{
      void* newData = mm_malloc(size);
      if(newData == NULL){
        if(curr->size>=size){
          return ptr;
        }
      }else{
        size_t min = 0;
        if(size<=curr->size){
          min = size;
        }else{
          min = size;
        }
        memcpy(newData,ptr,min);
        mm_free(ptr);
        return newData;
      }
    }
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
    printf("Strt = %lx\n",(size_t) b);
    printf("Size = %lx\n",(size_t)b->size);
    printf("Free = %s\n",(b->free)?"true":"false");
    printf("Data = %lx\n",(size_t) b->data);
    printf("Next = %lx\n",(size_t) b->next);
    printf("Buddy = %lx\n",(size_t) b->buddy);
    printf("Merged Buddies = ");
    while(b->merged_buddy[i] && i < MAX_EXP) {
        printf("%lx, ",(size_t) b->merged_buddy[i]);
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
