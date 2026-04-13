# Compiler and flags
CC=gcc

# Build modes
PROD_CFLAGS = -O3 -g -Wall -Wextra -Wpedantic -std=gnu11
DEBUG_CFLAGS = -O0 -g -Wall -Wextra -Wpedantic -std=gnu11
ASAN_CFLAGS = -O0 -g -Wall -Wextra -Wpedantic -std=gnu11 -fsanitize=address
UBSAN_CFLAGS = -O0 -g -Wall -Wextra -Wpedantic -std=gnu11 \
	-fsanitize=undefined -fsanitize=float-divide-by-zero

CFLAGS ?= $(PROD_CFLAGS)

CORE_SRCS=core/main.c \
	core/bench/common.c \
	core/bench/perf_bench.c \
	core/bench/timer_bench.c \
	core/bench/backend.c \
	core/batch/batch.c \
	core/data_processing.c \
	core/metric.c \
	core/report.c \
	core/workload.c

WORKLOAD_SRCS := $(wildcard workloads/*.c)

SRCS := $(CORE_SRCS) $(WORKLOAD_SRCS)
OBJS=$(SRCS:.c=.o)

# Output binary
OUT=cyclops

# Default target
all: $(OUT)

# Debug target overrides CFLAGS
debug: CFLAGS := $(DEBUG_CFLAGS)
debug: clean $(OUT)

# Asan target overrides CFLAGS
asan: CFLAGS := $(ASAN_CFLAGS)
asan: clean $(OUT)

# Asan target overrides CFLAGS
ubsan: CFLAGS := $(UBSAN_CFLAGS)
ubsan: clean $(OUT)

# Link objects
$(OUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile sources
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

clear:
	rm -f $(OBJS) $(OUT)

.PHONY: all clean clear
