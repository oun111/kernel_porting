
CC := gcc

APP_ENTRY := kernel_porting_test_cases
APP_ENTRY_FLAGS := main.c -I. -DAPP_ENTRY=$(APP_ENTRY) -e $(APP_ENTRY) -nostartfiles

C_SRCS        := $(shell find ./ ! -name "main.c" -a -name "*.c" )
C_OBJS        := $(patsubst %.c,%.o,$(C_SRCS))

OBJS            := $(C_OBJS)

LDFLAG          := -Wall -g -pthread -lrt  -ldl  -L. -lkp
CFLAG_OBJS      := -Wall -Werror -I. -g  -fPIC -Wno-error=unused-function 
TARGET          := test
TARGET_LIB      := libkp.a
TARGET_SO       := libkp.so


.PHONY: all
all: $(TARGET_LIB) $(TARGET_SO) $(TARGET) 

$(TARGET_LIB):$(OBJS)
	$(AR) -r $@ $(OBJS)

$(TARGET_SO):$(OBJS)
	$(CC) $(OBJS) -shared -o $@

$(TARGET):
	$(CC) -o $@ $(APP_ENTRY_FLAGS) $(LDFLAG)

$(C_OBJS):%.o:%.c
	$(CC) $(CFLAG_OBJS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET_SO) $(TARGET_LIB)

.PHONY: distclean
distclean:clean
	rm -rf cscope.* 
	rm -rf tags
