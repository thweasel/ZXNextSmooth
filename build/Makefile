TARGET_EXEC ?= program.bin

BUILD_DIR ?= ./bin
SRC_DIRS ?= ../src 

CC=zcc

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# zcc +zxn ../main.c -v -clib=new -create-app -subtype=nex -startup=5 -O2
CFLAGS=+zxn -v -clib=new -O3 
LDFLAGS=-startup=5 -pragma-include:../zpragma.inc -create-app -subtype=nex


INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Objects to binary
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# C source to Objects
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p