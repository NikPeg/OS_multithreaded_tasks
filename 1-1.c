#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
  printf("User identifier: %d\n", getuid());
  printf("Group identifier: %d\n", getgid());
  return 0;
}
