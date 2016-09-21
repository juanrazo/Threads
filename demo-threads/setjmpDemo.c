#include "stdio.h"
#include "setjmp.h"
jmp_buf jb;

void foo();

int main() 
{
  int n = 0;
  int r = setjmp(jb);
  printf("setjmp returned %d\n", r);
  if (r == 0)
    foo();
}

void foo() 
{
  longjmp(jb, 10);
}
