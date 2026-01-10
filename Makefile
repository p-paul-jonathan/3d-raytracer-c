# -------- Project --------
TARGET := raytracer
SRC    := main.c $(wildcard lib/*.c)
OBJ    := $(SRC:.c=.o)

# -------- Compiler --------
CC     := gcc
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic
INCLUDES := -Ilib

# -------- SDL3 --------
SDL_CFLAGS := $(shell pkg-config --cflags sdl3)
SDL_LIBS   := $(shell pkg-config --libs sdl3) -lm

# -------- Build --------
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(SDL_LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(SDL_CFLAGS) -c $< -o $@

# -------- Run --------
run: $(TARGET)
	./$(TARGET)

# -------- Clean --------
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: run clean
