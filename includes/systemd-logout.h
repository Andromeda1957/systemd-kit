#include <stdint.h>

#ifndef SYSTEMD_LOGOUT
#define SYSTEMD_LOGOUT

const size_t BUFFER_SIZE = 1024;
const uint16_t PORT = 10001;
int connfd;

void spawn_shell();
int handler(char option[BUFFER_SIZE]);
void read_data();
void start_server();

#endif