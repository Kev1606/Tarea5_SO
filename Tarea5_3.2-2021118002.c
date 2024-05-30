#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
#define BUF_SIZE 1024

void *receive_messages(void *arg) {
    int sock = *(int *)arg;
    char buffer[BUF_SIZE];

    while (1) {
        int read_size = recv(sock, buffer, BUF_SIZE, 0);
        if (read_size <= 0) {
            break;
        }
        buffer[read_size] = '\0';
        printf("%s", buffer);
    }

    return NULL;
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    pthread_t tid;
    pthread_create(&tid, NULL, receive_messages, (void *)&sock);

    while (1) {
        fgets(buffer, BUF_SIZE, stdin);
        send(sock, buffer, strlen(buffer), 0);
    }

    close(sock);
    return 0;
}
