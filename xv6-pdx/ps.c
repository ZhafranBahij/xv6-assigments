// borroiwng some formatting from date.c

#ifdef CS333_P2

#include "uproc.h"
#include "types.h"
#include "user.h"
#include "pdx.h"
#define HEADER1 "\nPID\tName\tUID\tGID\tPPID\tElapsed\tCPU\tState\tSize\t\n"
#define HEADER2 "\nPID\tName\tUID\tGID\tPPID\tPRIO\tElapsed\tCPU\tState\tSize\t\n"


int
main(int argc, char * argv[])
{
  int max = MAX;
  // allocate memory for as many processes as we specify as max in pdx.h
  struct uproc * table = malloc(sizeof(struct uproc)*max);
  int num = getprocs(max, table);
  char * error = "\n\tERROR WILL ROBINSON";

  // error
  if(num < 0){
    printf(1, error);
    return -1;
  }
  printf(1,HEADER1);

  for(int i = 0; i < num; ++i){
  printf(1,"%d\t%s\t%d\t%d\t%d\t%d.%d\t%d.%d\t%s\t%d\t\n", table[i].pid,table[i].name,
  table[i].uid,table[i].gid,table[i].ppid,table[i].elapsed_ticks/1000,table[i].elapsed_ticks%1000,
  table[i].CPU_total_ticks/1000,table[i].CPU_total_ticks%1000,table[i].state,table[i].size);
  }
  
  free(table);
  exit();

}

#endif
