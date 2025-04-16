# fastnetmon-vyos
The FastNetMon container and VyOS integration 
- [fastnetmon-vyos](#fastnetmon-vyos)
- [1. sflow-mode](#1-sflow-mode)
  - [1.1. pre-config](#11-pre-config)
  - [1.2. op-mode](#12-op-mode)
  - [1.3. configuration](#13-configuration)
- [2. mirror-mode](#2-mirror-mode)
  - [2.1. pre-config](#21-pre-config)
  - [2.2. op-mode](#22-op-mode)
  - [2.3. configuration](#23-configuration)
- [3. sflow-firewall](#3-sflow-firewall)
  - [3.1. build-container](#31-build-container)
  - [3.2. pre-config](#32-pre-config)
  - [3.3. configuration](#33-configuration)
  - [3.4. Check](#34-check)




# 1. sflow-mode

In the `sFlow` mode we get sFlow statistics from the VyOS router and send it to the FastNetMon container.
## 1.1. pre-config
Download required configuration files or make them manually.
```
# pre-config
mkdir -p /config/containers/fastnetmon

curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/fastnetmon_sflow.conf
curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/networks_list
curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/excluded_networks_list
curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/notify_about_attack.sh
```
## 1.2. op-mode
Add container image, before we can use it in the container configuration:
```
add container image ghcr.io/pavel-odintsov/fastnetmon-community:1.2.9
```
## 1.3. configuration
```
set system sflow interface 'eth0'
set system sflow interface 'eth1'
set system sflow server 127.0.0.1

set container name fastnetmon allow-host-networks
set container name fastnetmon command '/usr/bin/fastnetmon --configuration_file /etc/fastnetmon/fastnetmon_sflow.conf'
set container name fastnetmon image 'ghcr.io/pavel-odintsov/fastnetmon-community:1.2.9'
set container name fastnetmon memory '2048'
set container name fastnetmon volume configs destination '/etc/fastnetmon'
set container name fastnetmon volume configs source '/config/containers/fastnetmon'
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
set container name fastnetmon command '/usr/bin/fastnetmon --configuration_file /etc/fastnetmon/fastnetmon_mirror.conf'
set container name fastnetmon image 'ghcr.io/pavel-odintsov/fastnetmon-community:1.2.9'
set container name fastnetmon memory '2048'
set container name fastnetmon volume configs destination '/etc/fastnetmon'
set container name fastnetmon volume configs source '/config/containers/fastnetmon'
```
# 3. sflow-firewall
## 3.1. build-container
Build own container based on the official FastNetMon Dockerfile.

We need additional packages `iproute2` and `nftables` to add filters:
```
curl -O https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/Dockerfile

sudo podman build --net host --tag vyos-fastnetmon:1.2.9 -f Dockerfile
```
## 3.2. pre-config
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
## 3.3. configuration
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
## 3.4. Check

Firewall:
```
vyos@r14# sudo nft list table inet vyos_fastnetmon
table inet vyos_fastnetmon {
	set banned_ips {
		type ipv4_addr
		flags timeout
		elements = { 192.168.122.14 timeout 1h expires 59m40s114ms }
	}

	chain forward {
		type filter hook forward priority filter; policy accept;
		ip saddr @banned_ips counter packets 0 bytes 0 drop
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
