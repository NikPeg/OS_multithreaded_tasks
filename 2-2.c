#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
  pid_t pid = fork();
  if (pid == -1) {
    printf("An error\n");
  } else if (pid == 0) {
    for (int i = 0; i < 10; ++i)
      printf("I'm a child!\n");
  } else {
    for (int i = 0; i < 10; ++i)
      printf("I'm a parent!\n");
  }
}
