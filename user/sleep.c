/**
 * @file sleep.c
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

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "Arguments not enough.\n");
        exit(1);
    }

    int tick = atoi(argv[1]);
    sleep(tick);
    exit(0);
}
