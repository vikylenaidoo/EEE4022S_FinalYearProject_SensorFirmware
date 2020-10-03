.RECIPEPREFIX +=
CC = gcc
CFLAGS = -Wall

PROG = bin/*
OBJS = obj/*

default:
    mkdir -p bin obj
    $(CC) $(CFLAGS) -c src/serial.c -o obj/serial
    $(CC) $(CFLAGS) -c src/main.c -o obj/main
    $(CC) $(CFLAGS) obj/serial obj/main -o bin/main

run:
    sudo ./bin/main

clean:
    rm $(PROG) $(OBJS)