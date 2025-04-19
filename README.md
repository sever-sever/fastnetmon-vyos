# fastnetmon-vyos
The FastNetMon container and VyOS integration 
- [fastnetmon-vyos](#fastnetmon-vyos)
- [1. sflow-mode](#1-sflow-mode)
  - [1.1. build-container](#11-build-container)
  - [1.2. pre-config](#12-pre-config)
  - [1.3. configuration](#13-configuration)
  - [1.4. Check](#14-check)
- [2. mirror-mode](#2-mirror-mode)
  - [2.1. pre-config](#21-pre-config)
  - [2.2. op-mode](#22-op-mode)
  - [2.3. configuration](#23-configuration)


# 1. sflow-mode
## 1.1. build-container
Build own container based on the official FastNetMon Dockerfile.

We need additional packages `iproute2` and `nftables` to add filters:
```
curl -O https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/Dockerfile

sudo podman build --net host --tag vyos-fastnetmon:1.2.9 -f Dockerfile
```
## 1.2. pre-config
Download required configuration files or make them manually.
```
# pre-config
mkdir -p /config/containers/fastnetmon

curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/fastnetmon_sflow.conf
curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/networks_list
curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/excluded_networks_list
curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/notify_about_attack.sh

chmod +x /config/containers/fastnetmon/notify_about_attack.sh
```
## 1.3. configuration
```
set system sflow interface 'eth0'
set system sflow interface 'eth1'
set system sflow server 127.0.0.1

set container name fastnetmon allow-host-networks
set container name fastnetmon capability 'net-admin'
set container name fastnetmon command '/usr/bin/fastnetmon --configuration_file /etc/fastnetmon/fastnetmon_sflow.conf'
set container name fastnetmon image 'localhost/vyos-fastnetmon:1.2.9'
set container name fastnetmon memory '2048'
set container name fastnetmon volume configs destination '/etc/fastnetmon'
set container name fastnetmon volume configs source '/config/containers/fastnetmon'
```
## 1.4. Check
Wait for attack for some host, in example attack to the IP 192.168.122.14

Firewall:
```
vyos@r14# sudo nft list table inet vyos_fastnetmon
table inet vyos_fastnetmon {
        set banned_ips_v4 {
                type ipv4_addr
                flags timeout
                elements = { 192.0.2.1 timeout 1h expires 21m20s823ms }
        }

        set banned_ips_v6 {
                type ipv6_addr
                flags timeout
                elements = { 2001:db8::1 timeout 1h expires 22m35s473ms }
        }

        chain forward {
                type filter hook forward priority filter; policy accept;
                ip daddr @banned_ips_v4 counter packets 0 bytes 0 drop
                ip6 daddr @banned_ips_v6 counter packets 0 bytes 0 drop
        }
}
[edit]
vyos@r14#

```
Routing:
```
vyos@r14# run show ip route 192.168.122.14
Routing entry for 192.168.122.14/32
  Known via "kernel", distance 0, metric 0
  Last update 00:00:50 ago
  * unreachable (blackhole), weight 1

Routing entry for 192.168.122.14/32
  Known via "local", distance 0, metric 0, best
  Last update 11:07:39 ago
  * directly connected, eth0, weight 1

[edit]
vyos@r14#
```

# 2. mirror-mode
## 2.1. pre-config
Download required configuration files or make them manually.
```
# pre-config
mkdir -p /config/containers/fastnetmon

curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/fastnetmon_mirror.conf
curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/networks_list
curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/excluded_networks_list
curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/notify_about_attack.sh
```
## 2.2. op-mode
Add container image, before we can use it in the container configuration:
```
add container image ghcr.io/pavel-odintsov/fastnetmon-community:1.2.9
```
## 2.3. configuration
```
set container name fastnetmon allow-host-networks
set container name fastnetmon capability 'net-admin'
set container name fastnetmon command '/usr/bin/fastnetmon --configuration_file /etc/fastnetmon/fastnetmon_mirror.conf'
set container name fastnetmon image 'ghcr.io/pavel-odintsov/fastnetmon-community:1.2.9'
set container name fastnetmon memory '2048'
set container name fastnetmon volume configs destination '/etc/fastnetmon'
set container name fastnetmon volume configs source '/config/containers/fastnetmon'
```

