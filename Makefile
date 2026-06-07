# Compiler
CC = gcc

# Build modes
PROD_CFLAGS  = -O3 -g -Wall -Wextra -Wpedantic -std=gnu11
DEBUG_CFLAGS = -O0 -g -Wall -Wextra -Wpedantic -std=gnu11
ASAN_CFLAGS  = -O0 -g -Wall -Wextra -Wpedantic -std=gnu11 \
	-fsanitize=address
UBSAN_CFLAGS = -O0 -g -Wall -Wextra -Wpedantic -std=gnu11 \
	-fsanitize=undefined -fsanitize=float-divide-by-zero

CFLAGS ?= $(PROD_CFLAGS)

# Include path
CFLAGS += -Iinclude

# Core reusable sources (NO main.c)
CORE_SRCS = \
	core/cli.c \
	core/registry.c \
	core/metric.c \
	core/metric_grp/metric_grp.c \
	core/metric_grp/backend.c \
	core/metric_grp/backend_common.c \
	core/metric_grp/backend_perf.c \
	core/metric_grp/backend_isa.c \
	core/metric_grp/metric_grps/alignment_faults.c \
	core/metric_grp/metric_grps/arm_timer.c \
	core/metric_grp/metric_grps/bpu_reads.c \
	core/metric_grp/metric_grps/branch.c \
	core/metric_grp/metric_grps/dtlb_reads.c \
	core/metric_grp/metric_grps/ipc.c \
	core/metric_grp/metric_grps/itlb_reads.c \
	core/metric_grp/metric_grps/l1d_reads.c \
	core/metric_grp/metric_grps/l1i_reads.c \
	core/metric_grp/metric_grps/llc_reads.c \
	core/metric_grp/metric_grps/page_faults.c \
	core/metric_grp/metric_grps/rdtscp.c \
	core/metric_grp/metric_grps/stalled_cycles.c \
	core/metric_grp/metric_grps/task_clock_ns.c \
	core/experiment/batch.c \
	core/experiment/param_sweep.c \
	core/experiment/report.c \
	core/experiment/data_processing.c \
	core/workload.c \
	$(wildcard workloads/*.c)

# Main application sources
APP_SRCS = core/main.c $(CORE_SRCS)

# Object files
CORE_OBJS = $(CORE_SRCS:.c=.o)
APP_OBJS  = $(APP_SRCS:.c=.o)

# Application output
APP_BIN = cyclops

# Unit tests
UNIT_TEST_SRCS := $(wildcard tests/unit/*.c)

# Example:
# tests/unit/test_cli.c -> tests/unit/test_cli
UNIT_TEST_BINS := $(patsubst tests/unit/%.c,tests/unit/%.out,$(UNIT_TEST_SRCS))

# Default target
all: $(APP_BIN) $(UNIT_TEST_BINS)

# Build modes
debug: CFLAGS := $(DEBUG_CFLAGS) -Iinclude
debug: all

asan: CFLAGS := $(ASAN_CFLAGS) -Iinclude
asan: all

ubsan: CFLAGS := $(UBSAN_CFLAGS) -Iinclude
ubsan: all

# Main executable
$(APP_BIN): $(APP_OBJS)
	$(CC) $(CFLAGS) -o $@ $(APP_OBJS)

# Generic object compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Unit tests target
unit-tests: $(UNIT_TEST_BINS)

run-unit-tests: unit-tests
	for test in $(UNIT_TEST_BINS); do \
		echo "Running $$test"; \
		./$$test || exit 1; \
		echo "Passed test: $$test";\
	done

# Build each unit test executable
#
# Example:
# tests/unit/test_cli <- tests/unit/test_cli.c + CORE_OBJS
tests/unit/%.out: tests/unit/%.c $(CORE_OBJS)
	$(CC) $(CFLAGS) -o $@ $< $(CORE_OBJS)

# Cleanup
clean:
	rm -f $(APP_OBJS)
	rm -f $(CORE_OBJS)
	rm -f $(UNIT_TEST_BINS)

clear: clean
	rm -f $(APP_BIN)

.PHONY: all debug asan ubsan clean clear unit-tests
