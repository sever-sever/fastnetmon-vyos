#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "nft_operations.h"


/*
 * ip_version(ip) checks if the IP address is IPv4 or IPv6
 * returns: 4 or 6
 */
int ip_version(const char *src)
{
    char buf[sizeof(struct in6_addr)];
    if (inet_pton(AF_INET, src, buf))
    {
        return 4;
    }
    else if (inet_pton(AF_INET6, src, buf))
    {
        return 6;
    }

    return -1;
}

/*
 * ensure_nft_setup() adds the next nftables ruleset if it does not exist
 *
 * table inet vyos_fastnetmon {
 *        set banned_ips_v4 {
 *                type ipv4_addr
 *                flags timeout
 *        }
 *
 *        set banned_ips_v6 {
 *                type ipv6_addr
 *                flags timeout
 *        }
 *
 *        chain forward {
 *                type filter hook forward priority filter; policy accept;
 *                ip daddr @banned_ips_v4 counter drop
 *                ip6 daddr @banned_ips_v6 counter drop
 *        }
 * }
 */

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

    // Check if the nft IPv4 set exist
    snprintf(cmd, sizeof(cmd), "nft list set inet %s %s > /dev/null 2>&1", NFT_TABLE_NAME, NFT_SET_NAME_V4);
    if (system(cmd) != 0)
    {
        snprintf(cmd, sizeof(cmd), "nft add set inet %s %s { type ipv4_addr\\; flags timeout\\; }", NFT_TABLE_NAME, NFT_SET_NAME_V4);
        system(cmd);
    }

    // Check if the nft IPv6 set exist
    snprintf(cmd, sizeof(cmd), "nft list set inet %s %s > /dev/null 2>&1", NFT_TABLE_NAME, NFT_SET_NAME_v6);
    if (system(cmd) != 0)
    {
        snprintf(cmd, sizeof(cmd), "nft add set inet %s %s { type ipv6_addr\\; flags timeout\\; }", NFT_TABLE_NAME, NFT_SET_NAME_v6);
        system(cmd);
    }

    // Check if the nft rule exist for IPv4
    snprintf(cmd, sizeof(cmd), "nft -s list chain inet %s %s | grep -q '@%s counter drop'", NFT_TABLE_NAME, NFT_CHAIN_NAME, NFT_SET_NAME_V4);
    if (system(cmd) != 0)
    {
        snprintf(cmd, sizeof(cmd), "nft add rule inet %s %s ip daddr @%s counter drop", NFT_TABLE_NAME, NFT_CHAIN_NAME, NFT_SET_NAME_V4);
        system(cmd);
    }

    // Check if the nft rule exist for IPv6
    snprintf(cmd, sizeof(cmd), "nft -s list chain inet %s %s | grep -q '@%s counter drop'", NFT_TABLE_NAME, NFT_CHAIN_NAME, NFT_SET_NAME_v6);
    if (system(cmd) != 0)
    {
        snprintf(cmd, sizeof(cmd), "nft add rule inet %s %s ip6 daddr @%s counter drop", NFT_TABLE_NAME, NFT_CHAIN_NAME, NFT_SET_NAME_v6);
        system(cmd);
    }
}

void nft_ban_ip(char *ip)
{
    int ip_v = ip_version(ip);
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "nft add element inet %s %s_v%d { %s timeout %s }", NFT_TABLE_NAME, NFT_SET_NAME, ip_v, ip, NFT_BLOCK_TIMEOUT);
    system(cmd);
}

void nft_unban_ip(char *ip)
{
    int ip_v = ip_version(ip);
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "nft delete element inet %s %s_v%d { %s }", NFT_TABLE_NAME, NFT_SET_NAME, ip_v, ip);
    system(cmd);
}
