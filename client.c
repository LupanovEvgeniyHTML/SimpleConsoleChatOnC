#include <stdio.h>
#include <pthread.h>

#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "socket_error_proxy.c"

void * grabAndPrintEnterMessages(void * arg);

int fd;
// char serverAddress[256];
char serverAddress[256] = "127.0.0.1";

int main()
{
	// printf("Enter server address: ");
	// scanf("%s", serverAddress);

	fd = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET; // семейство адресов для IPv4
	addr.sin_port = htons(8011); // порт (по этому порту конектимся)

	Inet_pton(AF_INET, serverAddress, &addr.sin_addr);
	Connect(fd, (struct sockaddr *) &addr, sizeof addr);

	pthread_t thread;
	pthread_create(&thread, NULL, grabAndPrintEnterMessages, NULL);

	while(true)
	{
		char message[256];
		fgets(message, 256, stdin);
		message[strlen(message) - 1] = '\0'; // перенос строки убираю
		
		send(fd, message, strlen(message), 0);
	}

	close(fd);

	return 0;
}

void * grabAndPrintEnterMessages(void * arg)
{
	while (true)
	{
		char buf[256];
		recv(fd, buf, 256, 0);
		printf("<> message: %s\n", buf);
	}
}