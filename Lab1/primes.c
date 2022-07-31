#include "types.h"
#include "stat.h"
#include "user.h"
#define MSGSIZE 8
char *itoa(int value, char *result, int base) //converts int to string
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
    char inbuf[MSGSIZE]; //use for read from pipe
    //char* pim[40];
    char temp[40]; //use for write to pipe
    //int cnt = 0;
    //printf(2,"%d",atoi(argv[1]));
    int pid, i, p[2], cnt = 1, ro = 2, oti = 4;
    int *lal;
    int iqei = 0;
    lal = &iqei;
    //char  buf[MSGSIZE];
    pipe(p);
    if (argc != 2) //check usage of primes
    {
        printf(2, "usage: primes integer.. {doesnt hang up to 20}\n");
        exit();
    };
    itoa(ro, temp, 10); //convert first prime to string to send to pipe
    write(p[1], temp, MSGSIZE);

    for (i = 3; i < atoi(argv[1]); i++) //first pipe level to send primes not divisible by 1st prime 2
    {
        itoa(i, temp, 10);
        if (oti == 4) //save only the second prime
        {
            *lal = i;
            oti--; // be 3
        }
        if (i % ro != 0) //all not divisible by 2
        {

            cnt++; //count their count for next loop
            if ((pid = fork()) > 0)
            {
                write(p[1], temp, MSGSIZE);
                close(p[1]);
                wait();
            }
        }
    }

    close(p[1]);
    char klam[30][30], klam2[30][30];
    for (int kla = 0; kla < cnt; kla++)    //read from pipe and putting 1st sift in klam array
        read(p[0], klam[kla], MSGSIZE);
    wait();
    int r[2], cnt2 = 0, tam2 = 0;
    pipe(r); //init another pipe for next stage
    for (i = 0; i < cnt; i++)   //second filter stage 
    {
        tam2 = atoi(klam[i]);   //loop through klam array
        itoa(tam2, temp, 10);

        if (oti == 3)
        {
            oti--; // be 2
        }
        if (tam2 % (*lal) != 0 || tam2 == (*lal))  //if elements in klam not divisible by 3 send it to second pipe
        {
            cnt2++;
            if ((pid = fork()) > 0)
            {
                write(r[1], temp, MSGSIZE);
                close(r[1]);
                wait();
            }
        }
    }
    close(r[1]);    //for end of line 
    for (int kla2 = 0; kla2 < cnt2; kla2++)
        read(r[0], klam2[kla2], MSGSIZE);   //read from pipe and putting 2nd sift in klam2 array
    wait();

    int p2[2], cnt3 = 0, tam = 0;
    pipe(p2);   //3rd pipe
    for (i = 0; i < cnt2; i++)  //final stage for the 5 prime
    {
        tam = atoi(klam2[i]);
        itoa(tam, temp, 10);
        if (oti == 3)
        {
            oti--; // be 2
        }
        if (tam % (5) != 0 || tam == (5))
        {
            cnt3++;
            if ((pid = fork()) > 0)
            {
                write(p2[1], temp, MSGSIZE);
                close(p2[1]);
                wait();
            }
        }
    }

    close(p2[1]);
    for (int asda = 0; asda < cnt3; asda++)
    {
        read(p2[0], inbuf, MSGSIZE);
        if (strlen(inbuf) > 0)  //so it doesnt keep printing prime
        {
            printf(1, "prime %s\n", inbuf);
        }
        else
        {
            //printf(1,"here");
            close(p[1]);
            close(p2[1]);
            close(r[1]);
            close(p[0]);
            close(r[0]);
            close(p2[0]);
            exit();
            exit();
            return 0;
        }
    }
    exit();
    return 0;
}
