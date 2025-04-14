# fastnetmon-vyos
The FastNetMon container and VyOS integration 

# VyOS configuration
```
# pre-config
mkdir -p /config/containers/fastnetmon
curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/fastnetmon.conf
curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/networks_list
curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/excluded_networks_list
curl -O --output-dir /config/containers/fastnetmon/ https://raw.githubusercontent.com/sever-sever/fastnetmon-vyos/refs/heads/main/fastnetmon/notify_about_attack.sh

# sflow
set system sflow interface 'eth0'
set system sflow interface 'eth1'
set system sflow server 127.0.0.1

# op-mode
add container image ghcr.io/pavel-odintsov/fastnetmon-community:1.2.9

# conf-mode
set container name fastnetmon allow-host-networks
set container name fastnetmon capability 'net-admin'
set container name fastnetmon capability 'net-bind-service'
set container name fastnetmon capability 'net-raw'
set container name fastnetmon capability 'sys-admin'
set container name fastnetmon command '/usr/bin/fastnetmon --configuration_file /etc/fastnetmon/fastnetmon.conf'
set container name fastnetmon image 'ghcr.io/pavel-odintsov/fastnetmon-community:1.2.9'
set container name fastnetmon memory '2048'
set container name fastnetmon volume configs destination '/etc/fastnetmon'
set container name fastnetmon volume configs source '/config/containers/fastnetmon'
set container name fastnetmon volume log destination '/var/log/fastnetmon.log'
set container name fastnetmon volume log source '/var/log/fastnetmon.log'

```
