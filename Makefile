CC=gcc
CFLAGS=-Wall -lpthread
SRC_DIR=src
TEST_DIR=test_files


EXECUTABLES=main main_v2 main_v3


all: $(EXECUTABLES)


%: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(EXECUTABLES)

.PHONY: all clean