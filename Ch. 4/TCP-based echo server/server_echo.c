/* Server */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define MAXLINE 128 // maximal buffer size
#define MAXBACKLOG 10 // maximal backlog#

int send_Nbytes(int fd, const char *buf, int count)
{
	int r;
	int nleft = count;

	while (nleft > 0) {
		r = write(fd, buf, nleft);
		if (r < 0) {
			// error
			return -1;
		} else if (r == 0) {
			// this probably will never be executed
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
			// this probably will never be executed
			break;
		}

		nleft -= r;
		buf += r;
	}

	return count - nleft;
}

int main (int argc, char** argv)
{
	int sockfd_listen = 0;
	int sockfd_accept = 0;
	int ret = 0;
	int port = 0;
	int num_bytes = 0;
	char portstr[6];
	char recvbuf[MAXLINE];
	struct addrinfo hints;
	struct addrinfo *localaddr = NULL;
	int localaddr_len = 0;
    struct sockaddr_in peeraddr;
    socklen_t peeraddr_len;

	if (argc != 2) {
		printf("usage: server_echo <port number>\n");
		fflush(stdout);
		return 0;
	}

	port = atoi(argv[1]);

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	sprintf(portstr, "%d", port);

	if (getaddrinfo(NULL, portstr, &hints, &localaddr) != 0) {
        perror("local addr error");
		freeaddrinfo(localaddr);
		return -1;
	}

	if ((sockfd_listen = socket(localaddr->ai_family, localaddr->ai_socktype, 0)) < 0) {
		perror("socket create error");
		freeaddrinfo(localaddr);
		return -1;
	}

	if(bind(sockfd_listen, (struct sockaddr*)(localaddr->ai_addr), localaddr->ai_addrlen) < 0) {
		close(sockfd_listen);
		freeaddrinfo(localaddr);
		perror("socket bind error");
		return -1;
	} else {
		printf("bind local address - [%s|%d]\n",
			inet_ntoa( ((struct sockaddr_in*)localaddr->ai_addr)->sin_addr ),
			ntohs(((struct sockaddr_in*)localaddr->ai_addr)->sin_port) );
		fflush(stdout);
	}

	if (listen(sockfd_listen, MAXBACKLOG) < 0) {
		close(sockfd_listen);
		freeaddrinfo(localaddr);
		perror("socket listen error");
		return -1;
	} else {
		printf("listen on - [%s|%d]\n",
			inet_ntoa( ((struct sockaddr_in*)localaddr->ai_addr)->sin_addr ),
			ntohs(((struct sockaddr_in*)localaddr->ai_addr)->sin_port) );
		fflush(stdout);
	}

	peeraddr_len = sizeof(peeraddr);
	if((sockfd_accept = accept(sockfd_listen, (struct sockaddr*) &peeraddr, (socklen_t*)&peeraddr_len)) < 0) {
		close(sockfd_accept);
		perror("socket accept error");
		return -1;
	} else {
		printf("accepted connection from - [%s|%d]\n",
			inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
		fflush(stdout);
	}

	memset(recvbuf, '\0', MAXLINE);

	// tell me how many bytes you want to me to receive
	ret = recv_Nbytes(sockfd_accept, (char*)&num_bytes, sizeof(int));
	// printf("%d %d\n", ret, num_bytes); fflush(stdout);
	if(ret != sizeof(int) || num_bytes >= MAXLINE) {
		close(sockfd_accept);
		perror("receive size error");
		return -1;
	}

	ret = recv_Nbytes(sockfd_accept, recvbuf, num_bytes);
	// printf("%d %d\n", ret, num_bytes); fflush(stdout);
	if(ret != num_bytes) {
		close(sockfd_accept);
		perror("receive data error");
		return -1;
	}

	printf("received:\n%s\n", recvbuf); fflush(stdout);

	// wait for several seconds
	sleep(2);

	ret = send_Nbytes(sockfd_accept, recvbuf, num_bytes);
	if (ret != num_bytes){
		perror("send bytes error");
		return 0;
	}

	close(sockfd_listen);
	close(sockfd_accept);

	return 0;
}
