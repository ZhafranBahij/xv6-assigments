#ifdef CS333_P2
#include "types.h"
#include "user.h"

// Tambahan Tugas 2

int
main(int argc, char * argv [])
{
  int timer = uptime();
  int pid = fork();

  if(pid == 0){
    if(exec(argv[1], &argv[1])<0)
      printf(1, "\n\tDANGER WILL ROBINSION, ERROR!\n");
  }
  if(pid == -1){
      printf(1, "\n\tDANGER WILL ROBINSION, FAILURE!\n");
  }
  else{
    if(wait() >=0)
      printf(1, "%s ran in %d.%d seconds\n", argv[1], ((uptime() - timer)/1000), ((uptime() - timer)%1000));
  }
  exit();
}

#endif  //cs333_P2
