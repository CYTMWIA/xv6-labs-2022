/**
 * @file pingpong.c
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

char *fgets(int fd, char *buf, int max)
{
    int i, cc;
    char c;

    for (i = 0; i + 1 < max;)
    {
        cc = read(fd, &c, 1);
        if (cc < 1)
            break;
        buf[i++] = c;
        if (c == '\n' || c == '\r' || c == 0)
            break;
    }
    buf[i] = '\0';
    return buf;
}

int main()
{
    int p[2];
    if (pipe(p))
    {
        fprintf(2, "Fail to create pipe.");
        exit(1);
    }

    if (fork())
    {
        write(p[1], "ping", 5);

        wait(0);

        char buf[32];
        fgets(p[0], buf, 32);

        int pid = getpid();
        printf("%d: received %s\n", pid, buf);
    }
    else
    {
        // Child
        char buf[32];
        fgets(p[0], buf, 32);

        int pid = getpid();
        printf("%d: received %s\n", pid, buf);

        write(p[1], "pong", 5);
    }

    exit(0);
}
