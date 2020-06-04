#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int socketDesc;
  struct sockaddr_in server;
  char *message, server_reply[2000];

  // AF_INET - IPv4, SOCK_STREAM - tcp, 0 - IP
  socketDesc = socket(AF_INET, SOCK_STREAM, 0);

  if (socketDesc == -1) {
    printf("Não foi possivel criar o socket.\n");
    return 1;
  }

  server.sin_addr.s_addr = inet_addr("0.0.0.0");
  server.sin_family = AF_INET;
  server.sin_port = htons(80);

  if (connect(socketDesc, (struct sockaddr *)&server, sizeof(server)) == -1) {
    printf("Não foi possivel conectar-se.\n");
    return 1;
  }

  printf("Connected.\n");

  // Envia dados
  message = "GET / HTTP/1.1\r\n\r\n";
  if (send(socketDesc, message, strlen(message), 0) < 0) {
    printf("Falha ao enviar.\n");
    return 1;
  }

  printf("Dados enviados.\n");

  // Recebe resposta do servidor
  if (recv(socketDesc, server_reply, 2000, 0) < 0) {
    printf("Falha ao receber.\n");
    return 1;
  }

  printf("Resposta recebida.\n");
  puts(server_reply);

  // termina o socket
  close(socketDesc);

  return 0;
}
