#ifndef NFT_OPERATIONS

#define NFT_TABLE_NAME "vyos_fastnetmon"
#define NFT_SET_NAME "banned_ips"
#define NFT_CHAIN_NAME "forward"
#define NFT_HOOK_NAME "forward"
#define NFT_BLOCK_TIMEOUT "1h"

void ensure_nft_setup(void);
void nft_ban_ip(char *ip);
void nft_unban_ip(char *ip);

#endif