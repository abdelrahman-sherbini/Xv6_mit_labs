#include "types.h"
#include "stat.h"
#include "user.h"

int main (int argc,char *argv[]){

if (argc<2){printf(2,"error\n");exit();}



int x=atoi(argv[1]);
sleep(x);
exit();
}
