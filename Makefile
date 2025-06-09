CC = gcc
CFLAGS = -O2 $(shell pkg-config --cflags sdl2 SDL2_image)
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image)
SRC = src/main.c
OUT = dvd

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OUT)
