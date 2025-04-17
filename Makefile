CC = gcc
SRC_DIR = src
# SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/nft_operations.c $(SRC_DIR)/iproute_operations.c
CFLAGS = -Wall -Wextra
BIN_DIR = fastnetmon

SRC_NFT = $(SRC_DIR)/main_nft.c $(SRC_DIR)/nft_operations.c
SRC_IPROUTE = $(SRC_DIR)/main_iproute.c ${SRC_DIR}/iproute_operations.c

BIN_NFT = $(BIN_DIR)/nft_decision
BIN_IPROUTE = $(BIN_DIR)/iproute_decision


all: $(BIN_NFT) $(BIN_IPROUTE)
nft: $(BIN_NFT)
iproute: $(BIN_IPROUTE)

$(BIN_NFT): $(SRC_NFT)
	$(CC) $(CFLAGS) -o $@ $(SRC_NFT)

$(BIN_IPROUTE): $(SRC_IPROUTE)
	$(CC) $(CFLAGS) -o $@ $(SRC_IPROUTE)

clean:
	rm -f $(BIN_NFT) $(BIN_IPROUTE)

.PHONY: all clean
