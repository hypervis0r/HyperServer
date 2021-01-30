LIBDIR := lib/

INCLUDEDIR :=
INCLUDEDIR += -Iinclude/
INCLUDEDIR += -I$(LIBDIR)

CC := gcc
CFLAGS := $(INCLUDEDIR) -pedantic -Wall -Wextra -Werror -Wno-misleading-indentation -Wno-unused-parameter -Wno-unused-function

OBJS := hyper_server.o commands.o 

all: clean hyper-server
	@echo "Done!"

hyper-server: $(OBJS)
	$(CC) $^ -o $@ 

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o hyper-server
