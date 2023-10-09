.SILENT: all

CC=clang
CFLAGS=-Wall -Wextra -Wconversion -Wno-deprecated-declarations -s
INC=-Iincludes
SRC=systemd-logout.c
BIN=systemd-logout

all: systemd-logout.c systemd-daemon.c
	$(CC) $(CFLAGS) $(INC) systemd-logout.c -o systemd-logout
	$(CC) $(CFLAGS) $(INC) systemd-daemon.c -o systemd-daemon