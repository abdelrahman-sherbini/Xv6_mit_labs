#include "types.h"
#include "stat.h"
#include "user.h"

#define MSGSIZE 30
#define ARGS 3
char *rstrip(char *src)     //strip white space right
{
    int kk;
    for (kk = strlen(src); kk > 0; kk--)
    {
        if (src[kk] != '\x20')
            break;
    }

    src[kk + 1] = '\0';
    return src;
}
int main(int argc, char *argv[])
{

    int n, ar, pid, ite = 0;
    char buf[MSGSIZE];  //for read buffer
    static char *hky = '\0';
    while ((n = read(0, buf, sizeof(buf))) > 0) //loop through lines line by line
    {


        if ((pid = fork()) == 0)    //child
        {
            static char *args[ARGS];    //for exec args

            for (ar = 1; ar < argc; ar++)   //take args for xargs command
            {
                args[ar - 1] = argv[ar];
                for (int mh = 0; mh < n; mh++) //end line at first white space met
                {
                    if (buf[n - mh] == '\n')
                        buf[n - mh] = '\0';
                    else if (buf[strlen(buf) - 1] == '\32')
                        buf[strlen(buf) - 1] = '\0';
                }
                if (buf[strlen(buf) - 1] == '\n')
                    buf[strlen(buf) - 1] = '\0';
                args[ar] = buf;
                strcpy(hky, buf);
            }

            ite++;

            exec(argv[1], args);
            wait();
        }
        wait();

    }
    if (ite > 2)
        printf(1, "%s\n", hky);
    exit();
    return 0;
}
