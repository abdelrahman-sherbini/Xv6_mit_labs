//#include "types.h"
//#include "defs.h"
//#include "param.h"
//#include "memlayout.h"
//#include "mmu.h"
//#include "proc.h"
//#include "x86.h"
//#include "traps.h"
//#include "spinlock.h"
#include "types.h"
#include "stat.h"
#include "user.h"
//#include "unistd.h"
#define MSGSIZE 8
char *itoa(int value, char *result, int base) //converts int to char
{
    // check that the base if valid
    if (base < 2 || base > 36)
    {
        *result = '\0';
        return result;
    }

    char *ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do
    {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    } while (value);

    // Apply negative sign
    if (tmp_value < 0)
        *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

int main(int argc, char *argv[])
{
    char inbuf[MSGSIZE];

    int i, flag = 0, p[2], cnt = 0;
    char buf[MSGSIZE];
    pipe(p);
    if (argc != 2)  //check usage of primes
    {
        printf(2, "usage: primes integer..\n");
        exit();
    }
    //printf(2,"int : %u char : %u , float : %u",sizeof(int),sizeof(char),sizeof(float));
    for (i = 2; i <= atoi(argv[1]); i++)
    {
        flag = 0;
        //printf(2,"%d \n",i);
        for (int j = 2; j < i; j++)
        {

            if (i % j == 0)
            {
                //printf(2,"%d  %d:::",i,j);
                flag = 1;
                break;
            }
        }
        if (!flag)
        {
            cnt++;
            itoa(i, buf, 10);
            write(p[1], buf, MSGSIZE);
        }
    }
    close(p[1]);
    for (int t = 0; t < cnt; t++)
    {
        read(p[0], inbuf, MSGSIZE);
        printf(2, "prime %s\n", inbuf);
    }
    exit();
    return 0;
}
