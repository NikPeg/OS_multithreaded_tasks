#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
  pid_t pid = fork();
  if (pid == -1) {
    printf("An error\n");
  } else if (pid == 0) {
    execl("child_file", "child_file", NULL);
  } else {
    execl("parent_file", "parent_file", NULL);
  }
}
