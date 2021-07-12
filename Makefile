TARGET=main
LIBS=-lm -lSDL2 -lSDL2_ttf -lSDL2_image -lGL -lGLEW
CC=gcc
CFLAGS=-Iinclude -g -Wall

SDIR=src
ODIR=build/obj
IDIR=include

.PHONY: default all run clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(wildcard $(SDIR)/*.c))
HEADERS = $(wildcard $(IDIR)/*.h)

$(ODIR)/%.o: $(SDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

run: $(TARGET)
	./$^

clean:
	-rm -f $(ODIR)/*.o $(TARGET)
