CC = gcc
CFLAGS = -Wall -Wextra -std=c99
BUILD_DIR = build

all: $(BUILD_DIR)/client $(BUILD_DIR)/serveur

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/client: client.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/serveur: serveur.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)