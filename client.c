#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8888
#define DATA_MAXSIZE 1024

int main(int argc, char *argv[]) {
  int socket_desc;
  struct sockaddr_in server;
  char message[DATA_MAXSIZE], server_reply[DATA_MAXSIZE];

  if (argc != 2) {
    fprintf(stderr, "Usage: client <nickname>\n\nExample:\nclient marcosgz\n\n");
    return 1;
  }
  char *nickname = argv[1];

  // AF_INET - IPv4, SOCK_STREAM - tcp, 0 - IP
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_desc == -1) {
    puts("Ooops. It was not possible to create a socket.");
    return 1;
  }

  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);

  if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) == -1) {
    puts("Oops. Could not connect to the server.");
    return 1;
  }

  do {
    if (recv(socket_desc, server_reply, DATA_MAXSIZE, 0) < 0) {
      puts("[Internal] Oops. Coult not receve message from server");
      return 1;
    }
    printf("%s\n", server_reply);
    bzero(server_reply, sizeof(server_reply));

    printf("(Digite a mensagem OU \"exit\" para sair) > ");
    bzero(message, sizeof(message)); // clear the message
    int ch, n = 0;
    while ((ch = getchar()) != '\n' && n < DATA_MAXSIZE) {
      message[n] = ch;
      ++n;
    }

    char buf[DATA_MAXSIZE];
    snprintf(buf, sizeof buf, "[%s] %s", nickname, message);

    if (send(socket_desc, buf, strlen(buf), 0) < 0) {
      puts("[Internal] Oops. Could not send this message");
      return 1;
    }
  } while (strcmp(message, "exit") != 0);

  // termina o socket
  close(socket_desc);

  return 0;
}
