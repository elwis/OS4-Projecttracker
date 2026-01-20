CC = ppc-amigaos-gcc
CFLAGS = -O2 -Wall -Iinclude
LDFLAGS = -lauto -lsqlite3	
TARGET = Projecttracker

SRCDIR = src
SOURCES = $(SRCDIR)/main.c \
          $(SRCDIR)/gui/main_window.c \
		  $(SRCDIR)/db/database.c	

OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean