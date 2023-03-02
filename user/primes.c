/**
 * @file primes.c
 * @author CYTMWIA (cytmwia@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-02-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "kernel/types.h"
#include "user/user.h"

void sieve(int fd_in)
{
    int prime;
    read(fd_in, &prime, 1);

    if (prime)
        printf("prime %d\n", prime);
    else
        exit(1);

    int p[2];
    if (pipe(p))
    {
        fprintf(2, "Fail to create pipe.\n");
        exit(1);
    }

    if (fork())
    {
        int n = prime;
        while (n)
        {
            read(fd_in, &n, 1);
            if (n % prime)
                write(p[1], &n, 1);
        }
        n = 0;
        write(p[1], &n, 1);
        close(p[1]);
        close(fd_in);
        wait(0);
    }
    else
    {
        close(p[1]);
        sieve(p[0]);
    }
}

int main()
{
    int p[2];
    if (pipe(p))
    {
        fprintf(2, "Fail to create pipe.\n");
        exit(1);
    }

    int i;
    for (i = 2; i <= 35; i++)
        write(p[1], &i, 1);
    i = 0;
    write(p[1], &i, 1);
    close(p[1]);

    sieve(p[0]);

    exit(0);
}
