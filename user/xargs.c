/**
 * @file xargs.c
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
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    int cmd_argc = 0;
    char *cmd_argv[MAXARG];
    for (int i = 1; i < argc; i++)
    {
        cmd_argv[cmd_argc] = malloc(strlen(argv[i])+1);
        strcpy(cmd_argv[cmd_argc++], argv[i]);
    }

    char buf[100];
    for (int i = 0;; i++)
    {
        int cc = read(0, buf + i, 1);
        if (cc < 1)
        {
            buf[i] = 0;
            cmd_argv[cmd_argc] = malloc(strlen(buf)+1);
            strcpy(cmd_argv[cmd_argc++], buf);
            break;
        }
        if (buf[i] == ' ' || buf[i] == '\n')
        {
            if (i > 0)
            {
                buf[i] = 0;
                cmd_argv[cmd_argc] = malloc(strlen(buf)+1);
                strcpy(cmd_argv[cmd_argc++], buf);
            }
            i = -1;
        }
        if (cmd_argc >= MAXARG)
            break;
    }
    exec(cmd_argv[0], cmd_argv);
    exit(0);
}
