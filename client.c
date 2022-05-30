#include <stdio.h>
#include <pthread.h>

#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "socket_error_proxy.c"

void * grabAndPrintMessages(void * arg);
void * writeAndSendMessages(void * arg);

int fd;
const int bufferLen = 256;
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

	pthread_t threadWrite;
	pthread_create(&threadWrite, NULL, writeAndSendMessages, NULL);

	grabAndPrintMessages(NULL);

	close(fd);

	return 0;
}

void * grabAndPrintMessages(void * arg)
{
	char buf[bufferLen];
	int nread = bufferLen;
	while (true)
	{
		memset(buf, '\0', nread);

		nread = recv(fd, buf, bufferLen, 0);
		if (nread == 0) {
			printf("Server is gone ._.\n");
			break;
		}

		printf("<> message: %s\n", buf);
	}

	return NULL;
}

void * writeAndSendMessages(void * arg)
{
	char buf[bufferLen];
	int nread = bufferLen;
	while(true)
	{
		memset(buf, '\0', nread);

		fgets(buf, bufferLen, stdin);
		nread = strlen(buf);
		buf[--nread] = '\0'; // перенос строки убираю
		
		send(fd, buf, nread, 0);
	}

	return NULL;
}
