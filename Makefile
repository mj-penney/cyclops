# Compiler and flags
CC=gcc
CFLAGS?=$(BUILD_CFLAGS)

CORE_SRCS=core/main.c \
	core/bench.c \
	core/batch.c \
	core/data_processing.c \
	core/metric.c \
	core/report.c \
	core/workload.c

WORKLOAD_SRCS := $(wildcard workloads/*.c)

SRCS := $(CORE_SRCS) $(WORKLOAD_SRCS)

OBJS=$(SRCS:.c=.o)

# Output binary
OUT=out

# Default target
all: $(OUT)

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
