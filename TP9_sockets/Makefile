CC = gcc
CFLAGS = -Wall -Wextra -std=c99
BUILD_DIR = build

all: $(BUILD_DIR)/receiver $(BUILD_DIR)/emitter

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/receiver: question_1/receiver.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/emitter: question_1/emitter.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)