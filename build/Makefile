TARGET_EXEC ?= program.bin

BUILD_DIR ?= ./bin
SRC_DIRS ?= ../src

CC=zcc

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# zcc +zxn ../main.c -v -clib=new -create-app -subtype=nex -startup=5 -O2
CFLAGS=+zxn -vn -clib=new -compiler=sccz80 -O2 

#zcc +zx -vn -SO3 -startup=30 -clib=sdcc_iy --max-allocs-per-node200000 test.c -o test -create-app
#CFLAGS=+zxn -vn -SO3 -clib=sdcc_iy --max-allocs-per-node20000
LDFLAGS=-startup=5 -pragma-include:$(SRC_DIRS)/project/zpragma.inc -create-app -subtype=nex -m -lm

# COMPILER OPTIONS -compiler=sccz80 -compiler=sdcc

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