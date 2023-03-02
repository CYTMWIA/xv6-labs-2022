/**
 * @file find.c
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
#include "kernel/stat.h"
#include "kernel/fs.h"

char *basename(char *path)
{
    char *res = path;
    for (char *cc = path; *cc; cc++)
        if (*cc == '/')
            res = cc;
    return res + 1;
}

void find(char *path, char *target)
{
    struct stat st;
    if (stat(path, &st))
    {
        fprintf(2, "find: cannot stat %s\n", path);
        return;
    }
    if (st.type != T_DIR)
    {
        fprintf(2, "find: `%s` is not a dir.\n", path);
        return;
    }

    int fd;
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    struct dirent de;
    char *p = path + strlen(path);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
            continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(path, &st) < 0)
        {
            fprintf(2, "find: cannot stat %s\n", path);
            continue;
        }

        if (st.type == T_DIR)
        {
            if (strcmp("..", basename(path)) == 0 || strcmp(".", basename(path)) == 0)
                continue;
            find(path, target);
        }
        else
        {
            if (*target == 0 || strcmp(target, basename(path)) == 0)
                printf("%s\n", path);
        }
    }

    close(fd);
}

int main(int argc, char *argv[])
{
    char path[MAXPATH], target[MAXPATH];

    if (3 < argc)
    {
        fprintf(2, "Too many arguments.\n");
        exit(1);
    }
    else if (3 == argc)
    {
        strcpy(path, argv[1]);
        strcpy(target, argv[2]);
    }
    else if (2 == argc)
    {
        strcpy(path, ".");
        strcpy(target, argv[1]);
    }
    else
    {
        strcpy(path, ".");
        *target = 0;
    }

    find(path, target);
    exit(0);
}
