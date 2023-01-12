#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int fd1[2], fd2[2], result;
  size_t size;
  char child_resstring[14];
  char parent_resstring[14];
  if (pipe(fd1) < 0) {
    printf("Can\'t open the fist pipe\n");
    exit(-1);
  }
  if (pipe(fd2) < 0) {
    printf("Can\'t open the second pipe\n");
    exit(-1);
  }
  result = fork();
  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {
   /* Parent process */
    if (close(fd1[0]) < 0) {
      printf("parent: Can\'t close reading side of pipe\n"); exit(-1);
    }
    size = write(fd1[1], "Hello, child!", 14);
    if (size != 14) {
      printf("Can\'t write all string to pipe\n");
      exit(-1);
    }
    if (close(fd1[1]) < 0) {
      printf("parent: Can\'t close writing side of pipe\n"); exit(-1);
    }
    printf("Parent exit\n");
  } else {
    /* Child process */
    if (close(fd1[1]) < 0) {
      printf("child: Cna\'t close reading side of pipe\n");
    }
    if (close(fd2[0]) < 0) {
      printf("child: Can\'t close writing side of pipe\n"); exit(-1);
    }
    size = read(fd1[0], child_resstring, 14);
    if (size < 0) {
      printf("Can\'t read string from pipe\n");
      exit(-1);
    }
    printf("resstring:%s\n", child_resstring);
    size = write(fd2[1], "Hello, daddy!", 14);
    if (size != 14) {
      printf("Child: Can\'t write\n");
      exit(-1);
    }
    if (close(fd1[0]) < 0) {
      printf("child: Can\'t close reading side of pipe\n"); exit(-1);
    }
    printf("Exit child\n");
  }
  sleep(1);
  if (result > 0) {
    close(fd2[1]);
    read(fd2[0], parent_resstring, 14);
    printf("Parent resstring: %s\n", parent_resstring);
    close(fd2[0]);
    printf("Exit parent\n");
  }
  return 0;
}

