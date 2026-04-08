VERSION = 0.1

PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

CC = gcc

INCS = $(shell pkg-config --cflags sdl3)
LIBS = $(shell pkg-config --libs sdl3)

CFLAGS  = -O2 -Wall -Wextra -std=c11 -D_XOPEN_SOURCE=600 $(INCS)
LDFLAGS = $(LIBS) -lm

DBGFLAGS = --O0 -g -Wall -Wextra -std=c11 -D_XOPEN_SOURCE=600 $(INCS)

SRC = ch8.c main.c sdl.c
OBJ = $(SRC:.c=.o)

all: ch8

$(OBJ): ch8.h config.h sdl.h

ch8: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

debug: $(SRC)
	$(CC) $(DBGFLAGS) -o ch8-debug $(SRC) $(LDFLAGS)

clean:
	rm -f ch8 ch8-debug $(OBJ) ch8-$(VERSION).tar.gz

dist: clean
	mkdir -p ch8-$(VERSION)
	cp -R LICENSE Makefile README.md ch8.c ch8.h config.h main.c sdl.c sdl.h ch8-$(VERSION)
	tar -cf - ch8-$(VERSION) | gzip > ch8-$(VERSION).tar.gz
	rm -rf ch8-$(VERSION)

install: ch8
	mkdir -p $(DESTDIR)$(BINDIR)
	cp -f ch8 $(DESTDIR)$(BINDIR)/ch8
	chmod 755 $(DESTDIR)$(BINDIR)/ch8

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/ch8

.PHONY: all clean debug dist install uninstall run
