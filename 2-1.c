#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
  printf("Process identifier: %d\n", getpid());
  printf("Parent process identifier: %d\n", getppid());
  return 0;
}
