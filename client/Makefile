CC:=gcc
CFLAGS += -Wall -g 
TARGET = client

DIRS := $(shell find . -maxdepth 3 -type d) 
SOURCES = $(foreach dir,$(DIRS),$(wildcard $(dir)/*.c))
HEADERS = $(foreach	dir,$(DIRS),$(wildcard $(dir)/*.h))

OBJS := $(patsubst %.c,%.o, $(SOURCES))

.c.o:
		$(CC) -c  $(CFLAGS)   $< -o $@

LIBS +=  
$(TARGET):$(OBJS)
		$(CC) -o $@ $(OBJS) $(HEADERS) $(LIBS)

clean:
	rm -f *.o client
