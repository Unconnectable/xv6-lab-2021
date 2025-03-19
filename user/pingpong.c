#include "kernel/types.h"
#include "user/user.h"
#define READ_ 0
#define WRITE_ 1
int main() {
  int Pipe2[2];
  int Pipe1[2];
  char buf[100];

  if (pipe(Pipe1) || pipe(Pipe2)) {
    fprintf(2, "Pipe1 or Pipe2 failed\n");
    exit(1);
  }

  int pid = fork(); // 成功返回0 失败返回-1 否在是父进程

  if (pid < 0) {
    fprintf(2, "Fork failed\n");
    exit(1);
  }
  /*
                    0:read                    1:write

              child (read)  <---------- Parent(write)  this is pipe1
              Parent(read)  <---------- Child (write)  this is pipe2


  */
  else if (pid > 0) {
    /* 父亲进程
    坐标(Pipe,Function) WRITE_:1  READ_:0
    Parent process 只干和自己有关的事情 也就是
    Parent(write) Parent(read)
    也就是Pipe1[WRITE_] Pipe2[READ_]  坐标为(1,1) (2,0)
    和Child process有关的都关掉
    也就是(pipe1,0) (pipe2,1)
    */
    close(Pipe1[READ_]);
    close(Pipe2[WRITE_]); // 关掉Child Porcess的一端

    write(Pipe1[WRITE_], "ping", 4 + 1);
    close(Pipe1[WRITE_]); // 写入数据然后关掉

    read(Pipe2[READ_], buf, sizeof(buf));
    printf("%d: received %s\n",getpid(),buf); //read后printf
    close(Pipe2[READ_]);

    wait(0);
    exit(0);
  } else if (pid == 0) {
    // child process
    //close Parent process有关的(1,1) (2,0)

    close(Pipe1[1]);
    close(Pipe2[0]);

    //read数据
    read(Pipe1[0],buf,sizeof(buf));
    printf("%d: received %s\n",getpid(),buf);
    close(Pipe1[0]);

    write(Pipe2[1],"pong",4+1);
    close(Pipe2[1]);

    exit(0);
  }
  return 0;
}