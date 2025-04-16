#!/usr/bin/env bash

#
# This script will get following arguments from FastNetMon:
#
#  $1 IP of host which is under attack (incoming attack) or source of attack (outgoing attack)
#  $2 Attack direction: incoming or outgoing
#  $3 Attack bandwidth in packets per second
#  $4 Attack action: ban or unban
#

ip=$1
direction=$2
pps=$3
action=$4

# nft vars
table_name="vyos_fastnetmon"
set_name="banned_ips"
chain_name="forward"
hook_name="forward"
timeout="1h"


# Create nftables table and set if they don't exist
ensure_nft_setup() {
    # Create table if missing
    if ! nft list table inet "${table_name}" &>/dev/null; then
        nft add table inet "${table_name}"
    fi

    # Create chain if missing
    if ! nft list chain inet "${table_name}" "${chain_name}" &>/dev/null; then
        echo "[INFO] Creating chain ${chain_name} with hook"
        nft add chain inet "${table_name}" "${chain_name}" { type filter hook ${hook_name} priority 0\; }
    fi

    # Create set if missing
    if ! nft list set inet "${table_name}" "${set_name}" &>/dev/null; then
        nft add set inet "$table_name" "$set_name" { type ipv4_addr\; flags timeout\; }
    fi

    # Ensure drop rule exists
    if ! nft list chain inet "${table_name}" "${chain_name}" | grep -q "@${set_name} drop"; then
        nft add rule inet "${table_name}" "${chain_name}" ip saddr @"${set_name}" counter drop
    fi
}

add_ip_to_blackhole() {
    ip route add blackhole ${ip}/32
}

delete_ip_from_blackhole() {
    ip route del blackhole ${ip}/32
}

# action ban
if [ "${action}" = "ban" ]; then
    ensure_nft_setup
    nft add element inet "${table_name}" "${set_name}" { $ip timeout $timeout }
    add_ip_to_blackhole
    exit 0
fi

# action unban
if [ "${action}" = "unban" ]; then
    nft delete element inet "${table_name}" "${set_name}" { $ip }
    delete_ip_from_blackhole
    exit 0
fi
