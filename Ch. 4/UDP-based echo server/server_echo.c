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
	int i = 0;
	int sockfd;
	int ret = 0;
	int port = 0;
	int recv_bytes = 0;
	char portstr[6];
	// char recvbuf[MAXLINE];
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
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;
	sprintf(portstr, "%d", port);

	if (getaddrinfo(NULL, portstr, &hints, &localaddr) != 0) {
        perror("local addr error");
		freeaddrinfo(localaddr);
		return -1;
	}

	if ((sockfd = socket(localaddr->ai_family, localaddr->ai_socktype, 0)) < 0) {
		perror("socket create error");
		freeaddrinfo(localaddr);
		return -1;
	}

	if(bind(sockfd, (struct sockaddr*)(localaddr->ai_addr), localaddr->ai_addrlen) < 0) {
		close(sockfd);
		freeaddrinfo(localaddr);
		perror("socket bind error");
		return -1;
	} else {
		printf("bind local address - [%s|%d]\n",
			inet_ntoa( ((struct sockaddr_in*)localaddr->ai_addr)->sin_addr ),
			ntohs(((struct sockaddr_in*)localaddr->ai_addr)->sin_port) );
		fflush(stdout);
	}

	/* No listen for UDP
	if (listen(sockfd_listen, MAXBACKLOG) < 0) {
		close(sockfd_listen);
		freeaddrinfo(localaddr);
		perror("socket listen error");
		return -1;
	} else {
		printf("control channel listen on - [%s|%d]\n",
			inet_ntoa( ((struct sockaddr_in*)localaddr->ai_addr)->sin_addr ),
			ntohs(((struct sockaddr_in*)localaddr->ai_addr)->sin_port) );
		fflush(stdout);
	}
	*/

	/* No accept for UDP
	peeraddr_len = sizeof(peeraddr);
	if((sockfd_accept = accept(sockfd_listen, (struct sockaddr*) &peeraddr, (socklen_t*)&peeraddr_len)) < 0) {
		close(sockfd_accept);
		perror("socket accept error");
		return -1;
	} else {
		printf("accepted connection from - [%s|%d]\n",
			inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
		fflush(stdout);
	}*/


	// receive 100 integer numbers
	for(i = 0; i < 200000000; i++){
		ret = recv_Nbytes(sockfd, (char*)&recv_bytes, sizeof(int));
		// do some checking to make sure I am reall receiving integers (4 bytes)
		if(ret != sizeof(int)) {
			close(sockfd);
			perror("receive size error");
			return -1;
		} else if (i != recv_bytes) {
			// this means there is packet loss
			printf("%d\n", recv_bytes);
		}

		// tell them what I've really received
		// printf("%d\n", recv_bytes);
		fflush(stdout);

		// wait for several seconds
		// sleep(0.5);
	}

	close(sockfd);

	return 0;
}
