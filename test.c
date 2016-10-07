#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>

int main(void){
  void* test = sbrk(0);
  printf("\n%p\n",test);
  void* test2 = sbrk(1024);
  printf("\n%p\n",test2);
  return 0;
}
