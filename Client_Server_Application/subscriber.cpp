#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "helpers.h"
#include <sys/types.h>

using namespace std;

struct mesaj_TCP {
    char type[50];
    int len;
    char payload[1600];
};

int main(int argc, char *argv[])
{
	int sockfd, n, ret;
	struct sockaddr_in serv_addr;

    int fdmax;
    int i;
    fd_set set, temp;
    FD_ZERO(&set);
    mesaj_TCP mesaj;
    char buffer[100];

    mesaj_TCP trimis;
    memset(&trimis, 0, sizeof(mesaj_TCP));
    strcpy(trimis.type, "conectare");
    strcpy(trimis.payload, argv[1]);
    trimis.len = strlen(trimis.payload);
    if (trimis.len > 10) { //id prea mare
        printf("Id is too long.\n");
        return 0;
    }

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");

    fdmax = sockfd;
    FD_SET(sockfd, &set);
    FD_SET(0, &set);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[3]));
	ret = inet_aton(argv[2], &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton");

	ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "connect");
    
    n = send(sockfd, &trimis, sizeof(mesaj_TCP), 0); // trimit id-ul dupa ce m-am conectat
    DIE(n <= 0, "send");

    while (1) {
        temp = set;
        
		ret = select(fdmax + 1, &temp, NULL, NULL, NULL);
		DIE(ret < 0, "select");

        for (i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &temp)) {
                if (i == sockfd) { //mesaj de la server
                    memset(&mesaj, 0, sizeof(mesaj_TCP));
                    ret = recv(i, &mesaj, sizeof(mesaj_TCP), 0);
                    DIE(ret < 0, "recv");
                    if (ret == 0) { //s-a inchis server-ul
                        close(sockfd);
                        return 0;
                    }
                    printf("%s\n", mesaj.payload);
                } else { //mesaj de la stdin
                    memset(&mesaj, 0, sizeof(mesaj_TCP));
		            memset(buffer, 0, 100);
		            fgets(buffer, BUFLEN - 1, stdin);
                    if (strncmp(buffer, "exit", 4) == 0) { // inchidere client
                        close(sockfd);
                        return 0;
                    }
                    sscanf(buffer, "%s %s", mesaj.type, mesaj.payload);
                    mesaj.len = strlen(mesaj.payload);
                    send(sockfd, &mesaj, sizeof(mesaj_TCP), 0);
                }
            }
        }
    }

	return 0;
}
