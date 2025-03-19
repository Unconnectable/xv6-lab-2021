//#include "kernel/stat.h" //不能加这行
#include "kernel/types.h"
#include "user/user.h"
int main(int argc, char *argv[]) {
  // sleep 10 only have two args

  if (argc != 2) {
    fprintf(2, "Usage: sleep times(seconds)...\n");
    exit(1);
  }

  // if(sleep)

  int sleep_times = atoi(argv[1]);
  sleep(sleep_times);

  exit(0);
}