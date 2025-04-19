#ifndef NFT_OPERATIONS

#define NFT_TABLE_NAME "vyos_fastnetmon"
#define NFT_SET_NAME "banned_ips"
#define NFT_SET_NAME_V4 NFT_SET_NAME "_v4"
#define NFT_SET_NAME_v6 NFT_SET_NAME "_v6"
#define NFT_CHAIN_NAME "forward"
#define NFT_HOOK_NAME "forward"
#define NFT_BLOCK_TIMEOUT "1h"

int ip_version(const char *src);

void ensure_nft_setup(void);
void nft_ban_ip(char *ip);
void nft_unban_ip(char *ip);

#endif
