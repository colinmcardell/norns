ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
  endif
	TARGET_EXTENSION=exe
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=out
endif

.PHONY: clean all run test

# Path definitions
PATHU = ../../third-party/unity/src/
PATHS = ../src/
PATHH = ../src/hardware/
PATHT = ./
PATHB = build/
PATHD = build/depends/
PATHO = build/objs/
PATHR = build/results/

BUILD_PATHS = $(PATHB) $(PATHD) $(PATHO) $(PATHR)

# Source file discovery
SRC_EVENT_SYSTEM = $(PATHS)event_system.c
SRC_HAL = $(PATHH)hal.c
SRCT = $(wildcard $(PATHT)test_*.c)
SRCH = $(PATHT)test_helpers.c
OBJS = $(patsubst $(PATHT)%.c,$(PATHO)%.o,$(SRCT)) $(PATHO)test_helpers.o

# Test executable
TARGET = $(PATHB)matron_tests.$(TARGET_EXTENSION)

# Results files
RESULTS = $(PATHR)test_results.txt

# Tools
CC = gcc
COMPILE = $(CC) -c
LINK = $(CC)
DEPEND = $(CC) -MM -MG -MF
CFLAGS = -Wall -std=c11 -g -O0 -I$(PATHS) -I$(PATHH) -I$(PATHU) -I. -DUNITY_INCLUDE_CONFIG_H -DTESTING
LDFLAGS = -pthread -lm

# Default target
all: $(BUILD_PATHS) $(TARGET)

# Building the test executable
$(TARGET): $(OBJS) $(PATHO)unity.o $(PATHO)event_system.o $(PATHO)hal.o
	$(LINK) -o $@ $^ $(LDFLAGS)

# Object file compilation rules
$(PATHO)%.o:: $(PATHT)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHO)event_system.o: $(SRC_EVENT_SYSTEM)
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHO)hal.o: $(SRC_HAL)
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHO)unity.o: $(PATHU)unity.c
	$(COMPILE) $(CFLAGS) $< -o $@

# Running the tests
test: $(BUILD_PATHS) $(TARGET) $(RESULTS)
	@echo "-----------------------\nTEST RESULTS:\n-----------------------"
	@cat $(RESULTS)
	@echo "\nDONE"

$(RESULTS): $(TARGET)
	-./$(TARGET) > $@ 2>&1

# Create build directories
$(PATHB):
	$(MKDIR) $(PATHB)

$(PATHD):
	$(MKDIR) $(PATHD)

$(PATHO):
	$(MKDIR) $(PATHO)

$(PATHR):
	$(MKDIR) $(PATHR)

# Clean up build artifacts
clean:
	$(CLEANUP) $(PATHO)*.o
	$(CLEANUP) $(PATHB)*.$(TARGET_EXTENSION)
	$(CLEANUP) $(PATHR)*.txt

.PRECIOUS: $(PATHB)%.$(TARGET_EXTENSION)
.PRECIOUS: $(PATHD)%.d
.PRECIOUS: $(PATHO)%.o
.PRECIOUS: $(PATHR)*.txt