#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "helpers.h"
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <netinet/tcp.h>

using namespace std;

struct mesaj_UDP {
    char topic[50];
    unsigned char tip_date;
    char continut[1500];
};

struct mesaj_TCP {
    char type[50];
    int len;
    char payload[1600];
};

int main(int argc, char *argv[])
{
	int listen_TCP, listen_UDP, newsockfd, portno;
	struct sockaddr_in serv_addr, cli_addr;
	int n, i, ret;
	socklen_t clilen;

    mesaj_UDP anunt;
    char mesaj[1600];
    mesaj_TCP venit;
    map<int, string> socket_id;
    map<string, vector<int>> topic_socket;
    char buffer[100];

	fd_set read_fds;	// multimea de citire folosita in select()
	fd_set tmp_fds;		// multime folosita temporar
	int fdmax;			// valoare maxima fd din multimea read_fds

	// se goleste multimea de descriptori de citire (read_fds) si 
    //multimea temporara (tmp_fds)
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

    //TCP
	listen_TCP = socket(AF_INET, SOCK_STREAM, 0);
	DIE(listen_TCP < 0, "socket");

	portno = atoi(argv[1]);
	DIE(portno == 0, "atoi");

	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	ret = bind(listen_TCP, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
	DIE(ret < 0, "bind");
	ret = listen(listen_TCP, 100);
	DIE(ret < 0, "listen");
	FD_SET(listen_TCP, &read_fds);
	fdmax = listen_TCP;

    //UDP
    listen_UDP = socket(AF_INET, SOCK_DGRAM, 0);
	DIE(listen_UDP < 0, "socket");
    ret = bind(listen_UDP, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
	DIE(ret < 0, "bind");
    FD_SET(listen_UDP, &read_fds);
    fdmax = listen_UDP;

    //adaug stdin in set pentru comanda de exit
    FD_SET(0, &read_fds);

	while (1) {
		tmp_fds = read_fds; 
		
		ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(ret < 0, "select");

		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {
                if (i == 0) { //se da o comanda server-ului de la tastatura
		            memset(buffer, 0, 100);
		            fgets(buffer, BUFLEN - 1, stdin);
                    if (strncmp(buffer, "exit", 4) == 0) { // inchidere server
                        for (i = 0; i <= fdmax; i++) {
                            if (FD_ISSET(i, &read_fds)) {
                                close(i);
                            }
                        }
                        return 0;
                    }
                }
				else if (i == listen_TCP) { //cerere de conexiune TCP
					clilen = sizeof(cli_addr);
					newsockfd = accept(listen_TCP, (struct sockaddr *) &cli_addr, &clilen);
					DIE(newsockfd < 0, "accept");

                    int var = 1; //flag necesar pt a scoate Neagle
                    ret = setsockopt(newsockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &var, sizeof(int));
                    DIE(ret < 0, "Neagle");

					// adaug noul client
					FD_SET(newsockfd, &read_fds);
					if (newsockfd > fdmax) { 
						fdmax = newsockfd;
					}                  
				} 
                else if (i == listen_UDP) { //mesaj de la un client UDP
                    memset(&anunt, 0, sizeof(mesaj_UDP));
                    clilen = sizeof(cli_addr);
                    ret = recvfrom(i, &anunt, sizeof(mesaj_UDP), 0, 
                                    (struct sockaddr*) &cli_addr, &clilen);
                    if (anunt.tip_date == 0) { //INT
                        int info;
                        memcpy(&info, anunt.continut + 1, 4);
                        info = ntohl(info);
                        if (anunt.continut[0] == 1) {
                            info *= -1;
                        }
                        memset(mesaj, 0, 1600);
                        sprintf(mesaj, "%s:%d - %s - INT - %d", 
                            inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), 
                            anunt.topic, info);
                    }
                    if (anunt.tip_date == 1) { //SHORT_REAL
                        uint16_t info;
                        memcpy(&info, anunt.continut, 2);
                        info = ntohs(info);
                        memset(mesaj, 0, 1600);
                        sprintf(mesaj, "%s:%d - %s - SHORT_REAL - %f", 
                            inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), 
                            anunt.topic, (double) info / 100);
                    }
                    if (anunt.tip_date == 2) { //FLOAT
                        uint32_t mare;
                        uint8_t mic;
                        double info;
                        memcpy(&mare, anunt.continut + 1, 4);
                        memcpy(&mic, anunt.continut + 5, 1);
                        info = mare;
                        info = ntohl(info);
                        for (i = 0; i < mic; i++) {
                            info /= 10;
                        }
                        if (anunt.continut[0] == 1) {
                            info *= -1;
                        }
                        memset(mesaj, 0, 1600);
                        sprintf(mesaj, "%s:%d - %s - FLOAT - %f", 
                            inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), 
                            anunt.topic, info);
                    }
                    if (anunt.tip_date == 3) { //STRING
                        memset(mesaj, 0, 1600);
                        sprintf(mesaj, "%s:%d - %s - STRING - %s", 
                            inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), 
                            anunt.topic, anunt.continut);
                    }
                        //verific daca exista deja topic ul
                        std::map<string, vector<int>>::iterator it;
                        //verifica daca exista topic
                        it = topic_socket.find(anunt.topic);
                        //daca exista trimit la oameni, daca nu nu
                        if (it != topic_socket.end()) {
                            vector<int> aux = topic_socket[anunt.topic];
                            for (int x : aux) {
                                memset(&venit, 0, sizeof(mesaj_TCP));
                                strcpy(venit.type, "server");
                                strcpy(venit.payload, mesaj);
                                venit.len = strlen(venit.payload);
                                n = send(x, &venit, sizeof(mesaj_TCP), 0);
                            }
                        }
                } 
                else {
                    // s-au primit date pe unul din socketii de client,
					// asa ca serverul trebuie sa le receptioneze
                    memset(&venit, 0, sizeof(mesaj_TCP));
                    n = recv(i, &venit, sizeof(mesaj_TCP), 0);
                    DIE(n < 0, "recv");

					if (n == 0) {
						// conexiunea s-a inchis
                        cout << "Client " << socket_id[i] << " disconnected.\n";
						close(i);
						// se scoate din multimea de citire socketul inchis 
						FD_CLR(i, &read_fds);
                        socket_id.erase(i);
					} else {
                        if (strcmp(venit.type, "conectare") == 0) { //clientul isi trimite id-ul
                            printf("New client %s connected from %s:%d.\n", 
                                venit.payload, inet_ntoa(cli_addr.sin_addr), 
                                ntohs(cli_addr.sin_port));
                            socket_id.insert(pair<int, string>(i, venit.payload));
                        }
                        if (strcmp(venit.type, "subscribe") == 0) { //clientul da subscribe la un topic
                            std::map<string, vector<int>>::iterator it;
                            //verific daca exista topic
                            it = topic_socket.find(venit.payload);
                            //adaug topic daca nu exista
                            if (it == topic_socket.end()) {
                                topic_socket.insert(pair<string, vector<int>>
                                    (venit.payload, vector<int>()));
                            }
                            //caut daca clientul era deja abonat la topic
                            std::vector<int>::iterator vec_it;
                            vec_it = find(topic_socket[venit.payload].begin(), 
                                topic_socket[venit.payload].end(), i);
                            if (vec_it == topic_socket[venit.payload].end()) {// daca nu era il adaug
                                topic_socket[venit.payload].push_back(i);
                            }
                        }
                        if (strcmp(venit.type, "unsubscribe") == 0) { //clientul da unsubscribe
                            std::map<string, vector<int>>::iterator it;
                            //verifica daca exista topic
                            it = topic_socket.find(venit.payload);
                            //daca exista scot clientul din map
                            if (it != topic_socket.end()) {
                                //caut daca clientul era abonat
                                std::vector<int>::iterator vec_it;
                                vec_it = find(topic_socket[venit.payload].begin(), 
                                    topic_socket[venit.payload].end(), i);
                                if (vec_it != topic_socket[venit.payload].end()) { //daca clientul era abonat il scot
                                    topic_socket[venit.payload].erase(vec_it);
                                }
                            }
                        }
					}
                }
			}
		}
	}

	return 0;
}
