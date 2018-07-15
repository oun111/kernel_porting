
CC := gcc

C_SRCS        := $(shell find ./ -name "*.c" )
C_OBJS        := $(patsubst %.c,%.o,$(C_SRCS))

OBJS            := $(C_OBJS)

LDFLAG          := -Wall -g -pthread -lrt  -ldl 
CFLAG_OBJS      := -Wall -Werror -I. -g  -fPIC -Wno-error=unused-function
TARGET          := test


.PHONY: all
all: $(TARGET) 

$(TARGET):$(OBJS)
	$(CC) $(LDFLAG) $(OBJS) -o $@

$(C_OBJS):%.o:%.c
	$(CC) $(CFLAG_OBJS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET) 

.PHONY: distclean
distclean:clean
	rm -rf cscope.* 
	rm -rf tags
