#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUF_SIZE 1024

void handle_client(int client_sock) {
    char filename[BUF_SIZE];
    
    // Recibe el nombre del archivo desde el cliente
    int read_size = recv(client_sock, filename, BUF_SIZE, 0);
    if (read_size <= 0) {
        close(client_sock);
        return;
    }
    
    filename[read_size] = '\0';  // Asegura que el nombre del archivo sea una cadena válida
    printf("Solicitud de archivo: %s\n", filename);  // Mensaje de depuración

    // Abre el archivo en modo lectura binaria
    FILE *file = fopen(filename, "rb");
    if (!file) {
        char *msg = "Error: No se pudo abrir el archivo.\n";
        send(client_sock, msg, strlen(msg), 0);
    } else {
        char buffer[BUF_SIZE];
        int bytes_read;
        // Envía el contenido del archivo al cliente
        while ((bytes_read = fread(buffer, 1, BUF_SIZE, file)) > 0) {
            send(client_sock, buffer, bytes_read, 0);
        }
        fclose(file);
    }
    
    close(client_sock);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Crea el socket del servidor
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Asocia el socket con la dirección y el puerto especificados
    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    // Escucha conexiones entrantes
    listen(server_sock, 10);

    printf("Servidor de archivos escuchando en el puerto %d\n", PORT);

    // Acepta y maneja conexiones de clientes
    while ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len)) > 0) {
        handle_client(client_sock);
    }

    close(server_sock);
    return 0;
}
