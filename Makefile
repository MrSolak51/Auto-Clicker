OUT=APP/ClytronClick.exe
MAIN_CODE=SOURCE_CODE/main.c
CC=gcc

CFLAGS := `pkg-config --cflags gtk+-3.0`
LIBS := `pkg-config --libs gtk+-3.0`

all: build run

build:
	$(CC) $(MAIN_CODE) $(CFLAGS) -o $(OUT) $(LIBS)
run:
	.\$(OUT)