#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookies != NULL) {
        int i;
        memset(line, 0, LINELEN);
        strcpy(line, "Cookie: ");
        for (i = 0; i < cookies_count; i++) {
            if (i == 0) {
                sprintf(line + strlen("Cookie: "), "%s", cookies[i]);
                compute_message(message, line);
                continue;
            }
            memset(line, 0, LINELEN);
            if (i != cookies_count - 1) {
                sprintf(line, "%s; ", cookies[i]);
                compute_message(message, line);
            } else {
                sprintf(line, "%s\n", cookies[i]);
                compute_message(message, line);
            }
        }       
    }
    // Step 4: add final new line
    compute_message(message, "");
    free(line);
    return message;
}

char *compute_get_request_token(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookies != NULL) {
        int i;
        memset(line, 0, LINELEN);
        strcpy(line, "Authorization: Bearer ");
        for (i = 0; i < cookies_count; i++) {
            if (i == 0) {
                sprintf(line + strlen("Authorization: Bearer "), "%s", cookies[i]);
                compute_message(message, line);
                continue;
            }
            memset(line, 0, LINELEN);
            if (i != cookies_count - 1) {
                sprintf(line, "%s; ", cookies[i]);
                compute_message(message, line);
            } else {
                sprintf(line, "%s\n", cookies[i]);
                compute_message(message, line);
            }
        }       
    }
    // Step 4: add final new line
    compute_message(message, "");
    free(line);
    return message;
}

char *compute_delete_request_token(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL) {
        sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookies != NULL) {
        int i;
        memset(line, 0, LINELEN);
        strcpy(line, "Authorization: Bearer ");
        for (i = 0; i < cookies_count; i++) {
            if (i == 0) {
                sprintf(line + strlen("Authorization: Bearer "), "%s", cookies[i]);
                compute_message(message, line);
                continue;
            }
            memset(line, 0, LINELEN);
            if (i != cookies_count - 1) {
                sprintf(line, "%s; ", cookies[i]);
                compute_message(message, line);
            } else {
                sprintf(line, "%s\n", cookies[i]);
                compute_message(message, line);
            }
        }       
    }
    // Step 4: add final new line
    compute_message(message, "");
    free(line);
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char **body_data,
                            int body_data_fields_count, char **cookies, int cookies_count)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    char *body_data_buffer = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %shost", host);
    compute_message(message, line);    
    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    int i;
    for (i = 0; i < body_data_fields_count; i++) {
        strcpy(body_data_buffer + strlen(body_data_buffer), body_data[i]);
        if (i != body_data_fields_count - 1) {
            strcpy(body_data_buffer + strlen(body_data_buffer), "&");
        }
    }
    memset(line, 0, LINELEN);
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);
    memset(line, 0, LINELEN);
    sprintf(line, "Content-Length: %ld", strlen(body_data_buffer));
    compute_message(message, line);   
    
    // Step 4 (optional): add cookies
    if (cookies != NULL) {
        int i;
        memset(line, 0, LINELEN);
        strcpy(line, "Authorization: Bearer ");
        for (i = 0; i < cookies_count; i++) {
            if (i == 0) {
                sprintf(line + strlen("Authorization: Bearer "), "%s", cookies[i]);
                compute_message(message, line);
                continue;
            }
            memset(line, 0, LINELEN);
            if (i != cookies_count - 1) {
                sprintf(line, "%s; ", cookies[i]);
                compute_message(message, line);
            } else {
                sprintf(line, "%s\n", cookies[i]);
                compute_message(message, line);
            }
        }       
    }
    // Step 5: add new line at end of header
    compute_message(message, "");
    // Step 6: add the actual payload data
    memset(line, 0, LINELEN);
    compute_message(message, body_data_buffer);

    free(line);
    free(body_data_buffer);
    return message;
}
