#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc,char *argv[]){
if (argc<2){printf(2,"error\n");exit();}

 trace(atoi(argv[1]));
//int y = atoi(argv[1]);
//printf(1,"%d   %d\n",x,y);
static char * args[20];
for (int ar=3;ar<argc;ar++){

args[ar-3]=argv[ar];
}
exec(argv[2],args);
exit();
return 0;



}
