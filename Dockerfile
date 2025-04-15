FROM ghcr.io/pavel-odintsov/fastnetmon-community:1.2.9

# Install nftables and iproute2
RUN apt-get update && \
    apt-get install -y nftables iproute2 && \
    apt-get clean && rm -rf /var/lib/apt/lists/*
