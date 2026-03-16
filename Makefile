# Compiler and flags
CC=gcc
CFLAGS?=$(BUILD_CFLAGS)

# Source files
SRCS=src/main.c \
		 src/bench.c \
		 src/batch.c \
		 src/data_processing.c \
		 src/metric.c \
		 src/report.c \
		 src/workload.c \
		 src/wl_contiguous_array.c \
		 src/wl_scattered_array.c \
		 src/wl_strided_array.c \

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

