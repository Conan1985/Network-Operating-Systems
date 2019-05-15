/* Client */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define MAXLINE 128 // maximal buffer size

int send_Nbytes(int fd, const char *buf, int count)
{
	int r;
	int nleft = count;

	while (nleft > 0) {
		r = write(fd, buf, nleft);
		if (r < 0) {
			// error
			printf("%s %d\n", strerror(errno), fd);
			return -1;
		} else if (r == 0) {
			// this probably will never be run
			break;
		}

		nleft -= r;
		buf += r;
	}
	return count;
}

int recv_Nbytes(int fd, char *buf, int count)
{
	int r;
	int nleft = count;

	while (nleft > 0) {
		r = read(fd, buf, nleft);
		if (r < 0) {
			// error
			return -1;
		} else if (r == 0) {
			// this probably will never be run
			break;
		}

		nleft -= r;
		buf += r;
	}

	return count - nleft;
}

int main(int argc, char** argv)
{
	int i = 0;
	int sockfd = 0;
	int ret = 0;
	int num_bytes = 0;
	char portstr[6];
	// char sendbuf[MAXLINE] = "";
	// char recvbuf[MAXLINE] = "";
	struct addrinfo hints;
	struct addrinfo *localaddr = NULL;
	struct addrinfo *serveraddr = NULL;

	if (argc != 3) {
		printf("usage: client_echo <server ip> <port number>\n");
		fflush(stdout);
		return 0;
	}

	
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM; // for UDP
	hints.ai_flags = AI_PASSIVE;
	// sprintf(portstr, "%d", argv[2]);
	if (getaddrinfo(NULL, "0", &hints, &localaddr) != 0) {
		perror("get local address error");
		freeaddrinfo(localaddr);
		freeaddrinfo(serveraddr);
		return -1;
	}

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM; // for UDP
	hints.ai_flags = AI_PASSIVE;
	// sprintf(portstr, "%d", argv[2]);
	if (getaddrinfo(argv[1], argv[2], &hints, &serveraddr) != 0) {
		perror("get server address error");
		freeaddrinfo(localaddr);
		freeaddrinfo(serveraddr);
		return -1;
	}

	sockfd = socket(serveraddr->ai_family, serveraddr->ai_socktype, 0);
	if (sockfd < 0) {
		close(sockfd);
		perror("socket create error");
		freeaddrinfo(localaddr);
		freeaddrinfo(serveraddr);
		return -1;
	}

	if(bind(sockfd, (struct sockaddr*)(localaddr->ai_addr), localaddr->ai_addrlen) < 0) {
			close(sockfd);
			perror("socket bind error");
			freeaddrinfo(localaddr);
			freeaddrinfo(serveraddr);
			return -1;
	} else {
			printf("bind local address - [%s|%d]\n",
			  inet_ntoa( ((struct sockaddr_in*)localaddr->ai_addr)->sin_addr ),
			  ntohs(((struct sockaddr_in*)localaddr->ai_addr)->sin_port) );
			fflush(stdout);
	}

	// I am using write so please connect
	printf("connecting to... - [%s|%d]\n",
			inet_ntoa( ((struct sockaddr_in*)serveraddr->ai_addr)->sin_addr ),
			ntohs(((struct sockaddr_in*)serveraddr->ai_addr)->sin_port) );
	fflush(stdout);

	if (connect(sockfd, (struct sockaddr *) serveraddr->ai_addr, serveraddr->ai_addrlen) < 0) {
		close(sockfd);
		perror("socket connect error");
		freeaddrinfo(localaddr);
		freeaddrinfo(serveraddr);
		return -1;
	} else {
		printf("connected to - [%s|%d]\n",
			inet_ntoa( ((struct sockaddr_in*)serveraddr->ai_addr)->sin_addr ),
			ntohs(((struct sockaddr_in*)serveraddr->ai_addr)->sin_port) );
	}

	for (i = 0; i < 200000000; i++){
		// htonl(num_bytes);
		ret = send_Nbytes(sockfd, (char*)&i, sizeof(int));
		if (ret != sizeof(int)){
			perror("send size error");
			return 0;
		} else {
			//printf("%d\n", i);
		}
		//sleep(0.5);
	}

	close(sockfd);

	return 0;
}
