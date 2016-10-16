#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>

int main(void){
  //size_t s = sizeof(double);
  printf("%ul\n", (unsigned long)(1024*(sizeof(int))));
  return 0;
}
