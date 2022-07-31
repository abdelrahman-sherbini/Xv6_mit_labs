//#include "stdio.h"
#include "types.h"
#include "stat.h"
#include "user.h"
//#include "unistd.h"
#define MSGSIZE 16
char *msg1 = "Received ping";
char *msg2 = "Received pong";

int main()
{
    char inbuf[MSGSIZE];    //for read buffer
    int p[2], pid;
    int r[2];
    pipe(r);
    if (pipe(p) < 0)
        exit();

    /* continued */
    if ((pid = fork()) > 0)
    {
        write(p[1], msg1, MSGSIZE); //write msg1 in parent 

        close(p[1]);

        wait();
    }

    else
    {
        close(p[1]);
        read(p[0], inbuf, MSGSIZE); //read from first parent in child
        printf(1, "%s : %d   ", inbuf, getpid());

        write(r[1], msg2, MSGSIZE); //write msg2 in child
        wait();

        close(r[1]);
    }
    if (pid > 0)
    {
        close(r[1]);
        read(r[0], inbuf, MSGSIZE); //read from first parent in child
        printf(2, "%s : %d   ", inbuf, getpid());
    }
    return 0;
}
