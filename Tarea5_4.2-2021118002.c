#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char filename[BUF_SIZE];
    char buffer[BUF_SIZE];

    // Crea el socket del cliente
    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    // Conecta el cliente al servidor
    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Solicita el nombre del archivo
    printf("Ingrese el nombre del archivo que desea recibir: ");
    fgets(filename, BUF_SIZE, stdin);
    filename[strcspn(filename, "\n")] = 0;  // Elimina el salto de línea al final

    // Envía el nombre del archivo al servidor
    send(sock, filename, strlen(filename), 0);

    // Recibe y muestra el contenido del archivo
    int bytes_received;
    while ((bytes_received = recv(sock, buffer, BUF_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_received, stdout);
    }

    close(sock);
    return 0;
}
