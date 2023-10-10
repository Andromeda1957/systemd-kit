#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    if (geteuid() !=0)
        return 1;

    while (1)
        system("./systemd-logout");

    return 0;
}