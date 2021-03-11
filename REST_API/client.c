#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

    int i, len;
    char comanda[100];
    char username[100];
    char password[100];
    char **body_data = calloc(100, sizeof(char *));
    for (i = 0; i < 100; i++) {
        body_data[i] = calloc(1000, sizeof(char));
    }
    char **cookies = calloc(100, sizeof(char *));
    for (i = 0; i < 100; i++) {
        cookies[i] = calloc(200, sizeof(char));
    }
    char *aux;
    char **tokens = calloc(100, sizeof(char *));
    for (i = 0; i < 100; i++) {
        tokens[i] = calloc(500, sizeof(char));  
    }
    char cale[200];
    int id;
    char title[100];
    char author[100];
    char genre[100];
    int page_count;
    char publisher[100];

    while (1) {
        sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
        memset(comanda, 0, 100);
        scanf("%s", comanda);

        if (strcmp(comanda, "register") == 0) { //register
            printf("username=");
            scanf("%s", username);
            printf("password=");
            scanf("%s", password);
            memset(body_data[0], 0, 1000);
            sprintf(body_data[0], "{\"username\": \"%s\",\"password\": \"%s\"}", username, password);
            message = compute_post_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/tema/auth/register", "application/json", body_data, 1, NULL, 0);
            send_to_server(sockfd, message);
            free(message);
            response = receive_from_server(sockfd);
            aux = strstr(response, "HTTP/1.1 ");
            aux = aux + strlen("HTTP/1.1 ");
            sscanf(aux, "%d ", &id);
            if (id != 201) {
                printf("Utilizatorul exista deja.\n");
            } else {
                printf("Succes.\n");
            }
            free(response);
        } else if (strcmp(comanda, "login") == 0) { //login
            printf("username=");
            scanf("%s", username);
            printf("password=");
            scanf("%s", password);
            memset(body_data[0], 0, 1000);
            sprintf(body_data[0], "{\"username\": \"%s\",\"password\": \"%s\"}", username, password);
            message = compute_post_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/tema/auth/login", "application/json", body_data, 1, NULL, 0);
            send_to_server(sockfd, message);
            free(message);
            response = receive_from_server(sockfd);
            aux = strstr(response, "HTTP/1.1 ");
            aux = aux + strlen("HTTP/1.1 ");
            sscanf(aux, "%d ", &id);
            if (id != 200) {
                printf("Credentiale gresite.\n");
            } else {
                printf("Succes.\n");
            }
            //salvez cookie daca nu a esuat login
            if (id != 200) {
                continue;
            }
            memset(cookies[0], 0, 200);
            aux = strstr(response, "Set-Cookie: ");
            aux = aux + strlen("Set-cookie: ");
            strncpy(cookies[0], aux, 150);
            i = 0;
            len = 0;
            while (1) {
                if (cookies[0][i] != ';') {
                    len++;
                } else {
                    break;
                }
                i++;
            }
            memset(cookies[0] + len, 0, 150 - len);
            free(response);
        } else if (strcmp(comanda, "enter_library") == 0) { //enter
            message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/tema/library/access", NULL, cookies, 1);
            send_to_server(sockfd, message);
            free(message);
            response = receive_from_server(sockfd);
            aux = strstr(response, "HTTP/1.1 ");
            aux = aux + strlen("HTTP/1.1 ");
            sscanf(aux, "%d ", &id);
            if (id != 200) {
                printf("Nu sunteti autentificat.\n");
            } else {
                printf("Succes.\n");
            }
            //salvez token daca nu a esuat enter
            if (id != 200) {
                continue;
            }
            memset(tokens[0], 0, 500);
            aux = strstr(response, "{\"token\":\"");
            aux = aux + strlen("{\"token\":\"");
            strncpy(tokens[0], aux, 300);
            i = 0;
            len = 0;
            while (1) {
                if (tokens[0][i] != '"') {
                    len++;
                } else {
                    break;
                }
                i++;
            }
            memset(tokens[0] + len, 0, 300 - len);
            free(response);
        } else if (strcmp(comanda, "get_books") == 0) { //listare toate cartile
            message = compute_get_request_token("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/tema/library/books", NULL, tokens, 1);
            send_to_server(sockfd, message);
            free(message);
            response = receive_from_server(sockfd);
            aux = strstr(response, "HTTP/1.1 ");
            aux = aux + strlen("HTTP/1.1 ");
            sscanf(aux, "%d ", &id);
            if (id != 200) {
                printf("Nu aveti acces la biblioteca.\n");
            } else {
                printf("Succes.\n");
                aux = strstr(response, "Connection: keep-alive");
                aux = aux + strlen("Connection: keep-alive");
                printf("%s\n", aux);
            }
            free(response);
        } else if (strcmp(comanda, "get_book") == 0) { //caut o singura carte
            printf("id=");
            scanf("%d", &id);
            memset(cale, 0, 200);
            sprintf(cale, "/api/v1/tema/library/books/%d", id);
            message = compute_get_request_token("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", cale, NULL, tokens, 1);
            send_to_server(sockfd, message);
            free(message);
            response = receive_from_server(sockfd);
            aux = strstr(response, "HTTP/1.1 ");
            aux = aux + strlen("HTTP/1.1 ");
            sscanf(aux, "%d ", &id);
            if (id == 404) {
                printf("Nu exista cartea.\n");
            } else if (id == 200) {
                printf("Succes.\n");
                aux = strstr(response, "Connection: keep-alive");
                aux = aux + strlen("Connection: keep-alive");
                printf("%s\n", aux);
            } else {
                printf("Nu aveti acces la biblioteca.\n");
            }
            free(response);
        } else if (strcmp(comanda, "add_book") == 0) { // adaug o carte
            printf("title=");
            scanf("%s", title);
            printf("author=");
            scanf("%s", author);
            printf("genre=");
            scanf("%s", genre);
            printf("page_count=");
            scanf("%d", &page_count);
            printf("publisher=");
            scanf("%s", publisher);

            memset(body_data[0], 0, 1000);
            sprintf(body_data[0], "{\"title\": \"%s\",\"author\": \"%s\",\"genre\": \"%s\",\"page_count\": \"%d\",\"publisher\": \"%s\"}", title, author, genre, page_count, publisher);
            message = compute_post_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/tema/library/books", "application/json", body_data, 1, tokens, 1);
            send_to_server(sockfd, message);
            free(message);
            response = receive_from_server(sockfd);
            aux = strstr(response, "HTTP/1.1 ");
            aux = aux + strlen("HTTP/1.1 ");
            sscanf(aux, "%d ", &id);
            if (id != 200) {
                printf("Nu sunteti autentificat.\n");
            } else {
                printf("Succes.\n");
            }
            free(response);
        } else if (strcmp(comanda, "delete_book") == 0) { // sterg o carte
            printf("id=");
            scanf("%d", &id);
            memset(cale, 0, 200);
            sprintf(cale, "/api/v1/tema/library/books/%d", id);
            message = compute_delete_request_token("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", cale, NULL, tokens, 1);
            send_to_server(sockfd, message);
            free(message);
            response = receive_from_server(sockfd);
            aux = strstr(response, "HTTP/1.1 ");
            aux = aux + strlen("HTTP/1.1 ");
            sscanf(aux, "%d ", &id);
            if (id == 404) {
                printf("Nu exista cartea.\n");
            } else if (id == 500) {
                printf("Nu aveti acces la biblioteca.\n");
            } else {
                printf("Succes.\n");
            }
            free(response);

        } else if (strcmp(comanda, "logout") == 0) { //logout
            message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/tema/auth/logout", NULL, cookies, 1);
            send_to_server(sockfd, message);
            free(message);
            response = receive_from_server(sockfd);
            aux = strstr(response, "HTTP/1.1 ");
            aux = aux + strlen("HTTP/1.1 ");
            sscanf(aux, "%d ", &id);
            if (id != 200) {
                printf("Nu sunteti logat.\n");
            } else {
                printf("Succes.\n");
            }
            free(response);
        } else if (strcmp(comanda, "exit") == 0) { //exit
            printf("Se inchide clientul.\n");
            //dezalocari
            for (i = 0; i < 100; i++) {
                free(body_data[i]);
            }
            free(body_data);
                for (i = 0; i < 100; i++) {
                free(cookies[i]);
            }
            free(cookies);
            for (i = 0; i < 100; i++) {
                free(tokens[i]);
            }
            free(cookies);
            close(sockfd);
            return 0;
        } else {
            printf("Comanda gresita.\n");
        }
        close(sockfd);
    }
    
    return 0;
}
