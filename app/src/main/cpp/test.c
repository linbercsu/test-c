
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "test.h"

#define PORT 8098

static const char *REQUEST = "GET /nexus/ HTTP/1.1\r\n"
                             "Host: 13.234.231.18:8098\r\n"
                             "Connection: keep-alive\r\n"
                             "Pragma: no-cache\r\n"
                             "Cache-Control: no-cache\r\n"
                             "Upgrade-Insecure-Requests: 1\r\n"
                             "User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/98.0.4758.80 Mobile Safari/537.36\r\n"
                             "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
                             "Accept-Encoding: gzip, deflate\r\n"
                             "Accept-Language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7\r\n"
                             "Cookie: _ga=GA1.1.801398810.1644222869\r\n\r\n";

static int httpFetchResponse(HttpRequest *request) {

    int sock, got, have = 0;
    struct sockaddr_in serv_address;
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "13.234.231.18", &serv_address.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *) &serv_address, sizeof(serv_address)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    send(sock, REQUEST, strlen(REQUEST), 0);

    request->response = (char *) malloc(1024 * 1024);
    memset(request->response, 0, 1024 * 1024);

    got = read(sock, buffer, BUFFER_SIZE);
    if (got > 0) {
        memcpy(request->response + have, buffer, got);
        have += got;
    }

    request->responseSize = have;

    printf("http request successful.%d\n\n%s\n", have, request->response);

    return 0;
}

void httpInit(struct httpRequest *request, const char *url) {
    memset(request, 0, sizeof(HttpRequest));
    request->fetchResponse = &httpFetchResponse;
    size_t count = strlen(url) + 1;
    request->url = (char *) malloc(count);
    memcpy(request->url, url, count);
}

void httpRelease(HttpRequest *request) {
    if (request->url != NULL) {
        free(request->url);
        request->url = NULL;
    }

    if (request->response != NULL) {
        free(request->response);
        request->response = NULL;
    }
}

#ifndef __ANDROID__

int main() {
    HttpRequest request;
    httpInit(&request, "not used.");
    request.fetchResponse(&request);
    httpRelease(&request);
}

#endif