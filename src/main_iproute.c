#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iproute_operations.h"

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s <ip> <direction> <pps> <action>\n", argv[0]);
        return 1;
    }

    char *ip = argv[1];
    char *action = argv[4];

    if (strcmp(action, "ban") == 0)
    {
        iproute_add_ip_to_blackhole(ip);
    }

    if (strcmp(action, "unban") == 0)
    {
        iproute_delete_ip_from_blackhole(ip);
    }
}
