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
```
## 3.3. configuration
```
set system sflow interface 'eth0'
set system sflow interface 'eth1'
set system sflow server 127.0.0.1

set container name fastnetmon allow-host-networks
set container name fastnetmon command '/usr/bin/fastnetmon --configuration_file /etc/fastnetmon/fastnetmon_sflow.conf'
set container name fastnetmon image 'localhost/vyos-fastnetmon:1.2.9'
set container name fastnetmon memory '2048'
set container name fastnetmon volume configs destination '/etc/fastnetmon'
set container name fastnetmon volume configs source '/config/containers/fastnetmon'
```
