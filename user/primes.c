#include "kernel/types.h"
#include "user/user.h"

int judge_prime_true(int x) {
  /*
  9
  2 4
  3 9

  17
  */
  if (x <= 1) {
    return 0;
  }
  if (x == 2 || x == 3) {
    return 1;
  }
  for (int i = 2; i * i <= x; ++i) {
    if (x % i == 0) {
      return 0;
    }
  }
  return 1;
}
void print() {
  // default print
  printf("prime 2\n");
  printf("prime 3\n");
}
int main() {
  int Pipe[2];
  if (pipe(Pipe) == -1) {
    fprintf(2, "Fail to creadte Pipe\n");
    exit(1);
  }

  /*

  child (read)  <---------- Parent(write) pipe1

  */
  int pid = fork();
  if (pid < 0) {
    fprintf(2, "Fail to create fork\n");
    exit(1);
  } else if (pid > 0) {
    // Parent process

    close(Pipe[0]); // Child 写端关掉

    for (int i = 2; i < 36; ++i) {
      write(Pipe[1], &i, sizeof(int));
    }
    close(Pipe[1]);
    wait(0); //否则会遇到并发问题
    exit(0); //一定要退出 否则会sepc=0x00000000000000fe 非法指令 或 内存访问错误
  } else {
    // Child process
    close(Pipe[1]);
    int x;
    while (read(Pipe[0], &x, sizeof(int)) > 0) {
      read(Pipe[0], &x, sizeof(int)); // Err on read data
      if (judge_prime_true(x)) {
        printf("prime %d\n", x);
      }
    }
    close(Pipe[0]);
    exit(0);
  }
  return 0;
}