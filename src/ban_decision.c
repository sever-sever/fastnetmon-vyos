#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NFT_TABLE_NAME "vyos_fastnetmon"
#define NFT_SET_NAME "banned_ips"
#define NFT_CHAIN_NAME "forward"
#define NFT_HOOK_NAME "forward"
#define NFT_BLOCK_TIMEOUT "1h"

void ensure_nft_setup(void);
void nft_ban_ip(char *ip);
void nft_unban_ip(char *ip);
void iproute_add_ip_to_blackhole(char *ip);
void iproute_delete_ip_from_blackhole(char *ip);


int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s <ip> <direction> <pps> <action>\n", argv[0]);
        return 1;
    }

    char *ip = argv[1];
    // char *direction = argv[2];
    // char *pps = argv[3];
    char *action = argv[4];

    if (strcmp(action, "ban") == 0)
    {
        printf("BAN!\n");
        ensure_nft_setup();
        nft_ban_ip(ip);
        iproute_add_ip_to_blackhole(ip);
    }

    if (strcmp(action, "unban") == 0)
    {
        printf("UNBAN!\n");
        ensure_nft_setup();
        nft_unban_ip(ip);
        iproute_delete_ip_from_blackhole(ip);
    }
}



void ensure_nft_setup(void)
{
    char cmd[256];
    // Check if the nft table exist
    snprintf(cmd, sizeof(cmd), "nft list table inet %s > /dev/null 2>&1", NFT_TABLE_NAME);
    if (system(cmd) != 0) 
    {
        snprintf(cmd, sizeof(cmd), "nft add table inet %s", NFT_TABLE_NAME);
        system(cmd);
    }
    // Check if the nft chain exist
    snprintf(cmd, sizeof(cmd), "nft list chain inet %s %s > /dev/null 2>&1", NFT_TABLE_NAME, NFT_CHAIN_NAME);
    if (system(cmd) != 0)
    {
        snprintf(cmd, sizeof(cmd), "nft add chain inet %s %s { type filter hook %s priority 0\\; }", NFT_TABLE_NAME, NFT_CHAIN_NAME, NFT_HOOK_NAME);
        system(cmd);
    }
    // Check if the nft set exist
    snprintf(cmd, sizeof(cmd), "nft list set inet %s %s > /dev/null 2>&1", NFT_TABLE_NAME, NFT_SET_NAME);
    if (system(cmd) != 0)
    {
        snprintf(cmd, sizeof(cmd), "nft add set inet %s %s { type ipv4_addr\\; flags timeout\\; }", NFT_TABLE_NAME, NFT_SET_NAME);
        system(cmd);
    }
    // Check if the nft rule exist
    snprintf(cmd, sizeof(cmd), "nft -s list chain inet %s %s | grep -q '@%s counter drop'", NFT_TABLE_NAME, NFT_CHAIN_NAME, NFT_SET_NAME);
    if (system(cmd) != 0)
    {
        snprintf(cmd, sizeof(cmd), "nft add rule inet %s %s ip daddr @%s counter drop", NFT_TABLE_NAME, NFT_CHAIN_NAME, NFT_SET_NAME);
        system(cmd);
    }
}

void nft_ban_ip(char *ip)
{
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "nft add element inet %s %s { %s timeout %s }", NFT_TABLE_NAME, NFT_SET_NAME, ip, NFT_BLOCK_TIMEOUT);
    system(cmd);
}

void nft_unban_ip(char *ip)
{
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "nft delete element inet %s %s { %s }", NFT_TABLE_NAME, NFT_SET_NAME, ip);
    system(cmd);
}

void iproute_add_ip_to_blackhole(char *ip)
{
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "ip route add blackhole %s/32", ip);
    system(cmd);
}

void iproute_delete_ip_from_blackhole(char *ip)
{
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "ip route del blackhole %s/32", ip);
    system(cmd);
}

