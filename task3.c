#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[], char *envp[]) {
  printf("Count: %d\n", argc);
  for (int i = 0; i < argc; ++i) {
    printf("%c ", *argv[i]);
  }
  printf("\n");
  char next_char = *envp[0];
  int i = 0;
  while (envp[i] != NULL) {
    printf("%c ", *envp[i++]);
  }
}
