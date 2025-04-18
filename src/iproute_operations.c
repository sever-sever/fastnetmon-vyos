#include <stdio.h>
#include <stdlib.h>

#include "iproute_operations.h"

void iproute_add_ip_to_blackhole(char *ip)
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "ip route add blackhole %s", ip);
    system(cmd);
}

void iproute_delete_ip_from_blackhole(char *ip)
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "ip route del blackhole %s", ip);
    system(cmd);
}
