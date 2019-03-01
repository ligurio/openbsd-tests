CC	=  cc
CFLAGS	=  -I .
CFLAGS	+= -W -Wall -Wextra -Wfloat-equal
CFLAGS	+= -Wundef -Wpointer-arith -Wcast-align -Wshadow
CFLAGS	+= -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return
CFLAGS	+= -Wswitch-enum -Wunreachable-code -Winit-self
CFLAGS	+= -Wno-unused-parameter -Werror -pedantic -O3
LDFLAGS	=

BASE_SOURCES = utils.c test_list.c
SOURCES		= main.c $(BASE_SOURCES)
OBJS		= $(SOURCES:.c=.o)
EXECUTABLE	= runtest

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXECUTABLE) $(OBJS)

.PHONY: clean
