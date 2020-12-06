LIBDIR := lib/

INCLUDEDIR :=
INCLUDEDIR += -Iinclude/
INCLUDEDIR += -I$(LIBDIR)include/

CC := gcc
CFLAGS := $(INCLUDEDIR) -pedantic -Wall -Wextra -Werror -Wno-misleading-indentation

OBJS := hyper_server.o 

all: clean hyper-server
	@echo "Done!"

hyper-server: $(OBJS)
	$(CC) $< -o $@ -L$(LIBDIR) -lhyper 

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o hyper-server
