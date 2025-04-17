#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nft_operations.h"

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s <ip> <direction> <pps> <action>\n", argv[0]);
        return 1;
    }

    char *ip = argv[1];
    char *action = argv[4];

    ensure_nft_setup();
    if (strcmp(action, "ban") == 0)
    {
        nft_ban_ip(ip);
    }

    if (strcmp(action, "unban") == 0)
    {
        nft_unban_ip(ip);
    }
}
